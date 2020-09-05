#pragma once
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <stan/math/rev/fun/sum.hpp>
#include <fastad>

namespace adb {

struct SumFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        return x.sum();
    }

    adept::aReal operator()(const adept::aVector& x) const
    {
        return adept::sum(x);
    }

    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        return stan::math::sum(x);
    }

    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        return ad::sum(x);
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = Eigen::VectorXd::Ones(x.size());
    }

    std::string name() const { return "sum"; }

    void fill(Eigen::VectorXd& x) {
        for (int i = 0; i < x.size(); ++i) {
            x(i) = i;
        }
    }
};

} // namespace adb
