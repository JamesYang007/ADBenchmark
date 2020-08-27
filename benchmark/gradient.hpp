#pragma once
#include <Eigen/Dense>
#include <adept.h>
#include <fastad>

namespace adb {

template <class F>
inline void adept_gradient(const F& f,
                           const Eigen::VectorXd& x,
                           double& fx,
                           Eigen::VectorXd& grad_fx) 
{
    Eigen::Matrix<adept::adouble, Eigen::Dynamic, 1> x_ad(x.size());
    for (int i = 0; i < x.size(); ++i) {
        x_ad(i) = x(i);
    }
    adept::active_stack()->new_recording();
    adept::adouble fx_ad = f(x_ad);
    fx = fx_ad.value();
    fx_ad.set_gradient(1.0);
    adept::active_stack()->compute_adjoint();
    grad_fx.resize(x.size());
    for (int i = 0; i < x.size(); ++i) {
        grad_fx(i) = x_ad[i].get_gradient();
    }
}

template <class F>
inline void fastad_gradient(const F& f,
                            Eigen::VectorXd& x,
                            double& fx,
                            Eigen::VectorXd& grad_fx) 
{
    ad::VarView<double, ad::vec> x_ad(x.data(), 
                                      grad_fx.data(),
                                      x.size());
    auto expr = ad::bind(f(x_ad));
    ad::autodiff(expr);
}

} // namespace adb
