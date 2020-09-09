#pragma once
#include <string>
#include <functor/functor_base.hpp>

namespace adb {

struct ProdFuncBase: FuncBase
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
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
};

} // namespace adb
