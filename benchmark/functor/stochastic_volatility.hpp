#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <stan/math.hpp>

namespace adb {

struct StochasticVolatilityFuncBase
{
    template <class T>
    double normal_log_density(const Eigen::MatrixBase<T>& y,
                              double mu, 
                              double sigma) const {
        double z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - std::log(sigma);
    }

    template <class T, class S>
    double normal_log_density(const Eigen::MatrixBase<T>& y,
                              double mu, 
                              const Eigen::MatrixBase<S>& sigma) const {
        double z_sq = ((y.array() - mu) / sigma.array()).matrix().squaredNorm();
        return -0.5 * z_sq - sigma.array().log().sum();
    }

    double cauchy_log_density(double y,
                              double loc, 
                              double scale) const {
        auto c = (y - loc) / scale;
        return -std::log(scale * (1 + c * c));
    }

    // Way too complicated
    void derivative(const Eigen::VectorXd&,
                    Eigen::VectorXd&) const
    {}

    std::string name() const { return "stochastic_volatility"; }

    void fill(Eigen::VectorXd& x) {
        // x will be 2**N for N in [0, 14]
        // we want to go instead 2**N + 3 for N in [1, 14]
        size_t N = x.size() / 2;
        if (N < 2) N = 2;

        // w in R^N, b in R, sigma in R
        x = Eigen::VectorXd::Random(2 * N + 3);
        x(2*N+1) = std::abs(x(2*N+1)) + 0.1;    // make sigma positive

        y = Eigen::VectorXd::Random(N);
    }

protected:
    Eigen::VectorXd y;
};

//struct StochasticVolatilityFunc
//{
//    // NOTE: this is not meant to be used for other libraries,
//    // but must be at least defined to compile without changing code.
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        return x(0);
//    }
//
//    // NOTE: same as above
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        return x(0);
//    }
//
//    // STAN
//    auto operator()(Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
//    {
//        using namespace stan::math;
//        using vec_t = Eigen::Matrix<var, Eigen::Dynamic, 1>;
//        size_t N = (x.size() - 3) / 2;
//        Eigen::Map<vec_t> h_std(x.data(), N);
//        Eigen::Map<vec_t> h(x.data() + N, N);
//        auto& phi = x(2*N);
//        auto& sigma = x(2*N + 1);
//        auto& mu = x(2*N + 2);
//        h = h_std * sigma;
//        h[0] /= sqrt(1. - phi * phi);
//        h += mu * Eigen::VectorXd::Ones(N);
//        for (size_t i = 1; i < N; ++i) {
//            h[i] += phi * (h[i-1] - mu);
//        }
//        auto lp = normal_lpdf(y, 0., exp(h / 2.)) +
//                normal_lpdf(h_std, 0., 1.) +
//                cauchy_lpdf(sigma, 0., 5.) +
//                cauchy_lpdf(mu, 0., 10.) +
//                uniform_lpdf(phi, -1., 1.);
//
//#ifdef STOCHASTIC_VOLATILITY_BYTESIZE
//
//        auto stack = stan::math::ChainableStack::instance_;
//        
//        size_t vs_size = stack->var_stack_.size() *
//                         sizeof(std::decay_t<decltype(stack->var_stack_[0])>);
//        size_t vns_size = stack->var_nochain_stack_.size() *
//                          sizeof(std::decay_t<decltype(stack->var_nochain_stack_[0])>);
//        size_t vas_size = stack->var_alloc_stack_.size() *
//                          sizeof(std::decay_t<decltype(stack->var_alloc_stack_[0])>);
//        size_t malloc_size = stack->memalloc_.bytes_allocated();
//
//        std::cout << "STAN var_stack_ bytesize: " 
//                  << vs_size
//                  << std::endl;
//
//        std::cout << "STAN var_nochain_stack_ bytesize: " 
//                  << vns_size                   
//                  << std::endl;
//
//        std::cout << "STAN var_alloc_stack_ bytesize: " 
//                  << vas_size
//                  << std::endl;
//
//        std::cout << "STAN memalloc_.bytes_allocated bytesize: " 
//                  << malloc_size
//                  << std::endl;
//
//        std::cout << "STAN lower bound bytesize: " 
//                  << vs_size + vns_size + vas_size + malloc_size
//                  << std::endl;
//
//#endif
//
//        return lp;
//    }
//
//    // double
//    double operator()(Eigen::VectorXd& x) const
//    {
//        using vec_t = Eigen::VectorXd;
//        size_t N = (x.size() - 3) / 2;
//        Eigen::Map<vec_t> h_std(x.data(), N);
//        Eigen::Map<vec_t> h(x.data() + N, N);
//        auto& phi = x(2*N);
//        auto& sigma = x(2*N+1);
//        auto& mu = x(2*N+2);
//
//        h = h_std.array() * sigma;
//        h[0] /= std::sqrt(1. - phi * phi);
//        h.array() += mu;
//        for (size_t i = 1; i < N; ++i) {
//            h[i] += phi * (h[i-1] - mu);
//        }
//
//        return normal_log_density(y, 0., (h/2).array().exp().matrix()) +
//                normal_log_density(h_std, 0., 1.) +
//                cauchy_log_density(sigma, 0., 5.) +
//                cauchy_log_density(mu, 0., 10.) -
//                std::log(2.);
//    }
//};

} // namespace adb
