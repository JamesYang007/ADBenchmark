#pragma once
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <adolc/taping.h>
#include <adolc/adouble.h>
#include <adolc/drivers/drivers.h>
#include <cppad/cppad.hpp>
#include <Sacado_No_Kokkos.hpp>
#include <fastad>

namespace adb {

template <class F>
inline void adept_gradient(const F& f,
                           Eigen::VectorXd& x,
                           double& fx,
                           Eigen::VectorXd& grad_fx) 
{
    adept::aVector x_ad(x.size());
    for (int i = 0; i < x.size(); ++i) {
        x_ad[i] = x(i);
    }
    adept::active_stack()->new_recording();
    adept::aReal fx_ad = f(x_ad);
    fx = fx_ad.value();
    fx_ad.set_gradient(1.0);
    adept::active_stack()->compute_adjoint();
    for (int i = 0; i < x_ad.size(); ++i) {
        grad_fx(i) = x_ad[i].get_gradient();
    }
}

template <typename F>
void adolc_gradient(const F& f,
                    const Eigen::VectorXd& x,
                    double& fx,
                    Eigen::VectorXd& grad_fx) {
    trace_on(1);
    Eigen::Matrix<adouble, Eigen::Dynamic, 1> x_ad(x.size());
    for (int n = 0; n < x.size(); ++n) {
        x_ad(n) <<= x(n);
    }
    adouble y = f(x_ad);
    y >>= fx;
    trace_off();
    gradient(1, x.size(), &x(0), &grad_fx(0));
}

template <class F>
inline void cppad_gradient(const F& f,
                           const Eigen::VectorXd& x,
                           double& fx,
                           Eigen::VectorXd& grad_fx) 
{
    Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> x_ad(x.size());
    for (int n = 0; n < x.size(); ++n) {
        x_ad(n) = x[n];
    }
    Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> y(1);
    CppAD::Independent(x_ad);
    y[0] = f(x_ad);
    CppAD::ADFun<double> g = CppAD::ADFun<double>(x_ad, y);
    fx = Value(y[0]);
    Eigen::VectorXd w(1);
    w(0) = 1.0;
    grad_fx = g.Reverse(1, w);
}

template <typename F>
void sacado_gradient(const F& f,
                     const Eigen::VectorXd& x,
                     double& fx,
                     Eigen::VectorXd& grad_fx) 
{
    Eigen::Matrix<Sacado::Rad::ADvar<double>, Eigen::Dynamic, 1> x_ad(x.size());
    for (int n = 0; n < x.size(); ++n) {
        x_ad(n) = x[n];
    }
    fx = f(x_ad).val();
    Sacado::Rad::ADvar<double>::Gradcomp();
    for (int n = 0; n < x.size(); ++n) {
        grad_fx(n) = x_ad(n).adj();
    }
}

// STAN already provides their own gradient function

template <class ExprType>
inline void fastad_gradient(ExprType& expr,
                            double& fx,
                            Eigen::VectorXd& grad_fx) 
{
    grad_fx.setZero();
    fx = ad::autodiff(expr);
}

template <class F>
inline void double_gradient(const F& f,
                            const Eigen::VectorXd& x,
                            double& fx,
                            Eigen::VectorXd&)
{
    fx = f(x);
}

} // namespace adb
