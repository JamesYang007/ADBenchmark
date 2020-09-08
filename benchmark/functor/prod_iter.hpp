#pragma once
#include <string>
#include <Eigen/Dense>
#include <functor/prod.hpp>
//#include <adept.h>
//#include <adept_arrays.h>

namespace adb {

struct ProdIterFuncBase: ProdFuncBase
{
    std::string name() const { return "prod_iter"; }
};

//struct ProdIterFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        T product_x = 1;
//        for (int i = 0; i < x.size(); ++i) {
//            product_x *= x(i);
//        }
//        return product_x;
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        adept::aReal product_x = 1;
//        for (int i = 0; i < x.size(); ++i) {
//            product_x *= x(i);
//        }
//        return product_x;
//    }
//
//};

} // namespace adb
