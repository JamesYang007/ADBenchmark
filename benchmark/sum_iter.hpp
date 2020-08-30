#pragma once
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <fastad>

namespace adb {

struct SumIterFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T sum_x = 0;
        for (int i = 0; i < x.size(); ++i) {
            sum_x += x(i);
        }
        return sum_x;
    }

    adept::aReal operator()(const adept::aVector& x) const
    {
        adept::adouble sum_x = 0;
        for (int i = 0; i < x.size(); ++i) {
            sum_x += x(i);
        }
        return sum_x;
    }

    template <class T, class S>
    auto operator()(ad::VarView<T, S>& x) const
    {
        return ad::sum(counting_iterator<>(0),
                       counting_iterator<>(x.size()),
                       [&](size_t i) { return x[i]; });
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = Eigen::VectorXd::Ones(x.size());
    }

    std::string name() const { return "sum_iter"; }

    void fill(Eigen::VectorXd& x) {
        for (int i = 0; i < x.size(); ++i) {
            x(i) = i;
        }
    }
};

} // namespace adb
