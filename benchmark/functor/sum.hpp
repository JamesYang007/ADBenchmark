#pragma once
#include <string>
#include <functor/functor_base.hpp>

namespace adb {

struct SumFuncBase: FuncBase
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        return x.sum();
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = Eigen::VectorXd::Ones(x.size());
    }

    std::string name() const { return "sum"; }
};

} // namespace adb
