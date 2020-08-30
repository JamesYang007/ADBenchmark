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

struct ProdIterFunc
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
        adept::aReal product_x = 1;
        for (int i = 0; i < x.size(); ++i) {
            product_x *= x(i);
        }
        return product_x;
    }

    template <class T, class S>
    auto operator()(ad::VarView<T, S>& x) const
    {
        return ad::prod(counting_iterator<>(0),
                        counting_iterator<>(x.size()),
                        [&](size_t i) { return x[i]; });
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

    std::string name() const { return "prod_iter"; }

    void fill(Eigen::VectorXd& x) {
        const double val = std::pow(1e10, 1.0 / x.size());
        x.array() = val;
    }
};

} // namespace adb
