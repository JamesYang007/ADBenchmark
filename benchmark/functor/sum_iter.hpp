#pragma once
#include <string>
#include <functor/sum.hpp>

namespace adb {

struct SumIterFuncBase: SumFuncBase
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
    std::string name() const { return "sum_iter"; }
};

} // namespace adb
