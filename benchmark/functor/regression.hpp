#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <stan/math.hpp>

namespace adb {

struct RegressionFuncBase
{
    template <class T>
    double normal_log_density(const Eigen::MatrixBase<T>& y,
                              double mu, 
                              double sigma) const 
    {
        double z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - std::log(sigma);
    }

    // Way too complicated to manually-compute
    void derivative(const Eigen::VectorXd&,
                    Eigen::VectorXd&) const
    {}

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

//struct RegressionFunc
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
//    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
//    {
//        using namespace stan::math;
//        using vec_t = Eigen::Matrix<var, Eigen::Dynamic, 1>;
//        size_t N = (x.size() - 2);
//        Eigen::Map<const vec_t> w(x.data(), N);
//        auto& b = x(N);
//        auto& sigma = x(N + 1);
//        return normal_lpdf(y, multiply(X, w) + b * vec_t::Ones(1000), sigma) +
//                normal_lpdf(w, 0., 1.) +
//                normal_lpdf(b, 0., 1.) -
//                uniform_lpdf(sigma, 0.1, 10.);
//    }
//
//    // double
//    double operator()(const Eigen::VectorXd& x) const
//    {
//        using vec_t = Eigen::VectorXd;
//        size_t N = (x.size() - 2);
//        Eigen::Map<const vec_t> w(x.data(), N);
//        Eigen::Map<const Eigen::Matrix<double, 1, 1>> b(x.data() + N);
//        auto& sigma = x(N+1);
//        auto z = y - ((X*w).array() + b.array()).matrix();
//        return normal_log_density(z, 0., sigma) +
//                normal_log_density(w, 0., 1.) +
//                normal_log_density(b, 0., 1.) -
//                std::log(10. - 0.1);
//    }
//};

} // namespace adb
