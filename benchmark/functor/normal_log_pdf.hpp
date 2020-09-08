#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <stan/math.hpp>

namespace adb {

struct NormalLogPdfFuncBase
{
    template <typename T>
    T normal_log_density(const Eigen::Matrix<T, Eigen::Dynamic, 1>& y,
                         const T& mu, 
                         const T& sigma) const 
    {
        T z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - (y.size() * log(sigma));
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = (mu_ - x.array()) / (sigma_ * sigma_);
    }

    std::string name() const { return "normal_log_pdf"; }

    void fill(Eigen::VectorXd& x) const 
    {
        x = Eigen::VectorXd::LinSpaced(1, 1, x.size());
        x.array() -= x.size()/2;
        x /= (x.size() + 1);
    }

protected:
    double mu_ = -0.56;
    double sigma_ = 1.37;
};

//struct NormalLogPdfFunc
//{
//    template <class T>
//    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        T mu = mu_;
//        T sigma = sigma_;
//        return normal_log_density(x, mu, sigma);
//    }
//
//    adept::aReal operator()(const adept::aVector& x) const
//    {
//        adept::adouble lp = 0;
//        double neg_log_sigma = -std::log(sigma_);
//        for (int i = 0; i < x.size(); ++i) {
//            adept::adouble z = (x(i) - mu_) / sigma_;
//            lp += neg_log_sigma - 0.5 * z * z;
//        }
//        return lp;
//    }
//
//    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
//    {
//        stan::math::var mu = mu_;
//        stan::math::var sigma = sigma_;
//        return stan::math::normal_lpdf(x, mu, sigma);
//    }
//
//    double operator()(const Eigen::VectorXd& x) const
//    {
//        return normal_log_density(x, mu_, sigma_);
//    }
//};

} // namespace adb
