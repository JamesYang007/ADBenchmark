#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>

namespace adb {

struct ProdFuncBase
{
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

    void fill(Eigen::VectorXd& x) const {
        double val = std::pow(1e10, 1.0 / x.size());
        x.array() = val;
    }
};

//struct ProdFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        return x.prod();
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        return adept::product(x);
//    }
//
//};

} // namespace adb
