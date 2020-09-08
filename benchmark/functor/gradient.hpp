#pragma once
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <adolc/drivers/drivers.h>
#include <cppad/cppad.hpp>
#include <Sacado_No_Kokkos.hpp>
#include <fastad>

namespace adb {

template <class F>
inline void adept_gradient(const F& f,
                           Eigen::VectorXd& x,
                           adept::aVector& x_ad,
                           double& fx,
                           Eigen::VectorXd& grad_fx) 
{
    for (int i = 0; i < x.size(); ++i) {
        x_ad[i].set_value(x(i));
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

//template <typename F>
inline void adolc_gradient(//const F& f,
                           Eigen::VectorXd& x,
                           //Eigen::Matrix<adouble, Eigen::Dynamic, 1>& x_ad,
                           double& fx,
                           Eigen::VectorXd& grad_fx) {
    function(1, 1, x.size(), &x(0), &fx);
    gradient(1, x.size(), &x(0), &grad_fx(0));
}

inline void cppad_gradient(Eigen::VectorXd& w,
                           CppAD::ADFun<double>& g,
                           const Eigen::VectorXd& x,
                           double& fx,
                           Eigen::VectorXd& grad_fx) 
{
    fx = g.Forward(0, x)[0];
    grad_fx = g.Reverse(1, w);
}

template <typename F>
inline void sacado_gradient(const F& f,
                            const Eigen::VectorXd& x,
                            Eigen::Matrix<Sacado::Rad::ADvar<double>, Eigen::Dynamic, 1>& x_ad,
                            double& fx,
                            Eigen::VectorXd& grad_fx) 
{
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

template <class F>
inline void double_gradient(const F& f,
                            Eigen::VectorXd& x,
                            double& fx,
                            Eigen::VectorXd&)
{
    fx = f(x);
}

} // namespace adb
