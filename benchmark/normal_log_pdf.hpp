#pragma once
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <stan/math.hpp>
#include <fastad>

namespace adb {

struct NormalLogPdfFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T mu = mu_;
        T sigma = sigma_;
        return normal_log_density(x, mu, sigma);
    }

    adept::aReal operator()(const adept::aVector& x) const
    {
        adept::adouble lp = 0;
        double neg_log_sigma = -std::log(sigma_);
        for (int i = 0; i < x.size(); ++i) {
            adept::adouble z = (x(i) - mu_) / sigma_;
            lp += neg_log_sigma - 0.5 * z * z;
        }
        return lp;
    }

    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        stan::math::var mu = mu_;
        stan::math::var sigma = sigma_;
        return stan::math::normal_lpdf(x, mu, sigma);
    }

    template <class T, class S>
    auto operator()(const ad::VarView<T, S>& x) const
    {
        return ad::normal_adj_log_pdf(x, mu_, sigma_);
    }

    double operator()(const Eigen::VectorXd& x) const
    {
        return normal_log_density(x, mu_, sigma_);
    }

    template <typename T>
    T normal_log_density(const Eigen::Matrix<T, Eigen::Dynamic, 1>& y,
                         const T& mu, 
                         const T& sigma) const {
        T lp = 0;
        T neg_log_sigma = -log(sigma);
        for (int i = 0; i < y.size(); ++i) {
            T z = (y(i) - mu) / sigma;
            lp += neg_log_sigma - 0.5 * z * z;
        }
        return lp;
    }

    double normal_log_density(const Eigen::VectorXd& y,
                              double mu, 
                              double sigma) const {
        double z_sq = (y.array() - mu).matrix().squaredNorm() / (sigma * sigma);
        return -0.5 * z_sq - std::log(sigma);
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const
    {
        grad = (mu_ - x.array()) / (sigma_ * sigma_);
    }

    std::string name() const { return "normal_log_pdf"; }

    static void fill(Eigen::VectorXd& x) {
        for (int i = 0; i < x.size(); ++i) {
            x(i) = static_cast<double>(i + 1 - x.size()/2) / (x.size() + 1);
        }
    }

private:
    double mu_ = -0.56;
    double sigma_ = 1.37;
};

} // namespace adb
