#pragma once
#define ADEPT_FAST_EXPONENTIAL
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <stan/math/rev/fun/log_sum_exp.hpp>
#include <fastad>

namespace adb {

/*
 * Functor representing product of elements in a vector.
 * Only fastad, adept provide vectorized notation.
 */

struct LogSumExpFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T total = 0.0;
        for (int i = 0; i < x.size(); ++i) {
            total = log(exp(total) + exp(x(i)));
        }
        return total;
    }

    adept::aReal operator()(const adept::aVector& x) const
    {
        return adept::log(1. + adept::sum(adept::fastexp(x)));
    }

    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        using stan::math::log;
        using stan::math::exp;
        using stan::math::log_sum_exp;
        return log(1. + exp(log_sum_exp(x)));
    }

    template <class T, class S>
    auto operator()(ad::VarView<T, S>& x) const
    {
        return ad::log(1. + ad::sum(ad::exp(x)));
    }

    double operator()(const Eigen::VectorXd& x) const
    {
        return std::log(1. + x.array().exp().sum());
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        double denom = 1 + x.array().exp().sum();
        grad = x.array().exp() / denom;
    }

    std::string name() const { return "log_sum_exp"; }

    void fill(Eigen::VectorXd& x) {
        for (int i = 0; i < x.size(); ++i) {
            x(i) = i / static_cast<double>(x.size());
        }
    }
};

} // namespace adb
