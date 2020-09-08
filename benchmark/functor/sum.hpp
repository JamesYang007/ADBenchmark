#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>

namespace adb {

struct SumFuncBase
{
    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = Eigen::VectorXd::Ones(x.size());
    }

    std::string name() const { return "sum"; }

    void fill(Eigen::VectorXd& x) const 
    {
        x = Eigen::VectorXd::LinSpaced(1, 0, x.size()-1);
    }
};

//struct SumFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        return x.sum();
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        return adept::sum(x);
//    }
//};

} // namespace adb
