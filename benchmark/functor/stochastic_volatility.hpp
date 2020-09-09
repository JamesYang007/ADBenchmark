#pragma once
#include <string>
#include <functor/functor_base.hpp>
#include <util/counting_iterator.hpp>
#include <fastad>

namespace adb {

struct StochasticVolatilityFuncBase: FuncBase
{
    template <class T>
    T operator()(Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        using std::sqrt;
        using vec_t = Eigen::Matrix<T, Eigen::Dynamic, 1>;
        size_t N = (x.size() - 3) / 2;
        Eigen::Map<vec_t> h_std(x.data(), N);
        Eigen::Map<vec_t> h(x.data() + N, N);
        auto& phi = x(2*N);
        auto& sigma = x(2*N+1);
        auto& mu = x(2*N+2);

        h = h_std.array() * sigma;
        h[0] /= sqrt(1. - phi * phi);
        h.array() += mu;
        for (size_t i = 1; i < N; ++i) {
            h[i] += phi * (h[i-1] - mu);
        }

        return normal_log_density(y.template cast<T>(), 0., (h/2).array().exp().matrix()) +
                normal_log_density(h_std, 0., 1.) +
                cauchy_log_density(sigma, 0., 5.) +
                cauchy_log_density(mu, 0., 10.) -
                std::log(2.);
    }

    // FastAD
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = (x.size() - 3) / 2;
        ad::VarView<T, ad::vec> h_std(x.data(), x.data_adj(), N);
        ad::VarView<T, ad::vec> h(x.data() + N, x.data_adj() + N, N);
        ad::VarView<T> phi(x.data() + 2*N, x.data_adj() + 2*N);
        ad::VarView<T> sigma(x.data() + 2*N + 1, x.data_adj() + 2*N + 1);
        ad::VarView<T> mu(x.data() + 2*N + 2, x.data_adj() + 2*N + 2);
        auto tp = (
            h = h_std * sigma,
            h[0] /= ad::sqrt(1. - phi * phi),
            h += mu,
            ad::for_each(counting_iterator<>(1),
                         counting_iterator<>(N),
                         [&](size_t i) { return h[i] += phi * (h[i-1] - mu); })
        );
        auto lp = ad::normal_adj_log_pdf(y, 0., ad::exp(h / 2.)) 
                + ad::normal_adj_log_pdf(h_std, 0., 1.)
                + ad::cauchy_adj_log_pdf(sigma, 0., 5.)
                + ad::cauchy_adj_log_pdf(mu, 0., 10.) 
                + ad::uniform_adj_log_pdf(phi, -1., 1.);
        return (tp, lp);
    }

    template <class T>
    auto normal_log_density(const Eigen::MatrixBase<T>& y,
                            double mu, 
                            double sigma) const {
        using Scalar = typename T::Scalar;
        Scalar z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - y.size() * std::log(sigma);
    }

    template <class T, class S>
    auto normal_log_density(const Eigen::MatrixBase<T>& y,
                            double mu, 
                            const Eigen::MatrixBase<S>& sigma) const {
        using Scalar = typename T::Scalar;
        Scalar z_sq = ((y.array() - mu) / sigma.array()).matrix().squaredNorm();
        return -0.5 * z_sq - sigma.array().log().sum();
    }

    template <class T>
    T cauchy_log_density(const T& y,
                         double loc, 
                         double scale) const {
        using std::log;
        T c = (y - loc) / scale;
        return -log(scale * (1 + c * c));
    }

    // Use FastAD as a reference
    void derivative(Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    { 
        grad.setZero();
        ad::VarView<double, ad::vec> x_ad(x.data(), grad.data(), x.size());
        auto expr = ad::bind(this->operator()(x_ad));
        ad::autodiff(expr);
    }

    std::string name() const { return "stochastic_volatility"; }

    void fill(Eigen::VectorXd& x) {
        // x will be 2**N for N in [0, 14]
        // we want to go instead 2**N + 3 for N in [1, 14]
        size_t N = x.size() / 2;
        if (N < 2) N = 2;

        x = Eigen::VectorXd::Random(2 * N + 3);
        x(2*N+1) = std::abs(x(2*N+1)) + 0.1;    // make sigma positive

        y = Eigen::VectorXd::Random(N);
    }

protected:
    Eigen::VectorXd y;
};

} // namespace adb
