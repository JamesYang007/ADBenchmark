#pragma once
#include <string>
#include <functor/prod.hpp>

namespace adb {

struct ProdIterFuncBase: ProdFuncBase
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
    std::string name() const { return "prod_iter"; }
};

} // namespace adb
