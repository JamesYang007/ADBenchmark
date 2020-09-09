#pragma once
#include <string>
#include <functor/functor_base.hpp>
#include <fastad>

namespace adb {

struct RegressionFuncBase: FuncBase
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        using vec_t = Eigen::Matrix<T, Eigen::Dynamic, 1>;
        size_t N = (x.size() - 2);
        Eigen::Map<const vec_t> w(x.data(), N);
        Eigen::Map<const Eigen::Matrix<T, 1, 1>> b(x.data() + N);
        auto& sigma = x(N+1);
        auto z = y.template cast<T>() - ((X.template cast<T>()*w).array() + b(0)).matrix();
        return normal_log_density(z, 0., sigma) +
                normal_log_density(w, 0., 1.) +
                normal_log_density(b, 0., 1.) -
                std::log(10. - 0.1);
    }

    // FastAD
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = (x.size() - 2);
        ad::VarView<T, ad::vec> w(x.data(), x.data_adj(), N);
        ad::VarView<T> b(x.data() + N, x.data_adj() + N);
        ad::VarView<T> sigma(x.data() + N + 1, x.data_adj() + N + 1);
        return ad::normal_adj_log_pdf(y, ad::dot(X, w) + b, sigma) 
                + ad::normal_adj_log_pdf(w, 0., 1.)
                + ad::normal_adj_log_pdf(b, 0., 1.)
                + ad::uniform_adj_log_pdf(sigma, 0.1, 10.);
    }

    template <class T, class MType, class SType>
    auto normal_log_density(const Eigen::MatrixBase<T>& y,
                            const MType& mu, 
                            const SType& sigma) const 
    {
        using std::log;
        typename T::Scalar z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - y.size() * log(sigma);
    }

    // Use FastAD as reference
    void derivative(Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    { 
        grad.setZero();
        ad::VarView<double, ad::vec> x_ad(x.data(), grad.data(), x.size());
        auto expr = ad::bind(this->operator()(x_ad));
        ad::autodiff(expr);
    }

    std::string name() const { return "regression"; }

    void fill(Eigen::VectorXd& x) {
        // x will be 2**N for N in [0, 14]
        // we want to go instead 10 * [1, 2, ..., 15]
        size_t N = (std::log2(x.size()) + 1) * 10;

        // w in R^N, b in R, sigma in R
        x = Eigen::VectorXd::Random(N + 2);
        x(N+1) = std::abs(x(N+1)) + 0.1;    // make sigma positive

        X = Eigen::MatrixXd::Random(1000, N);
        y = Eigen::VectorXd::Random(1000);
    }

protected:
    Eigen::MatrixXd X;
    Eigen::VectorXd y;
};

} // namespace adb
