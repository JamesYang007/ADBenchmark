#pragma once
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <fastad>

namespace adb {

/*
 * Functor representing product of elements in a vector.
 * Only fastad, adept provide vectorized notation.
 */

struct ProdFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T product_x = 1;
        for (int i = 0; i < x.size(); ++i) {
            product_x *= x(i);
        }
        return product_x;
    }

    adept::aReal operator()(const adept::aVector& x) const
    {
        return adept::product(x);
    }

    template <class T, class S>
    auto operator()(ad::VarView<T, S>& x) const
    {
        return ad::prod(x);
    }

    double operator()(const Eigen::VectorXd& x) const
    {
        return x.prod();
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = Eigen::VectorXd::NullaryExpr(x.size(),
                [&](size_t i) { 
                    double val = 1;
                    for (int k = 0; k < x.size(); ++k) {
                        if (k != i) val *= x(i);
                    }
                    return val;
                });
    }

    std::string name() const { return "prod"; }

    void fill(Eigen::VectorXd& x) {
        const double val = std::pow(1e10, 1.0 / x.size());
        x.array() = val;
    }
};

} // namespace adb
