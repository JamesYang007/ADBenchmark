#pragma once
//#define ADEPT_FAST_EXPONENTIAL
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <stan/math/rev/fun/log_sum_exp.hpp>

namespace adb {

struct LogSumExpFuncBase
{
    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        double denom = x.array().exp().sum();
        grad = x.array().exp() / denom;
    }

    std::string name() const { return "log_sum_exp"; }

    void fill(Eigen::VectorXd& x) const {
        x = Eigen::VectorXd::LinSpaced(1, 0, x.size()-1) / x.size();
    }
};

//struct LogSumExpFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        return log(x.array().exp().sum());
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        return adept::log(adept::sum(adept::fastexp(x)));
//    }
//
//    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
//    {
//        using stan::math::log;
//        using stan::math::exp;
//        using stan::math::log_sum_exp;
//        return log_sum_exp(x);
//    }
//
//    double operator()(const Eigen::VectorXd& x) const
//    {
//        return std::log(x.array().exp().sum());
//    }
//};

} // namespace adb
