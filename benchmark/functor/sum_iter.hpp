#pragma once
#include <string>
#include <Eigen/Dense>
#include <functor/sum.hpp>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <fastad>

namespace adb {

struct SumIterFuncBase : SumFuncBase
{
    std::string name() const { return "sum_iter"; }
};

//struct SumIterFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        T sum_x = 0;
//        for (int i = 0; i < x.size(); ++i) {
//            sum_x += x(i);
//        }
//        return sum_x;
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        adept::adouble sum_x = 0;
//        for (int i = 0; i < x.size(); ++i) {
//            sum_x += x(i);
//        }
//        return sum_x;
//    }
//
//};

} // namespace adb
