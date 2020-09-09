#pragma once
#include <string>
#include <functor/functor_base.hpp>

namespace adb {

struct NormalLogPdfFuncBase: FuncBase
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T mu = mu_;
        T sigma = sigma_;
        return normal_log_density(x, mu, sigma);
    }

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

protected:
    double mu_ = -0.56;
    double sigma_ = 1.37;
};

} // namespace adb
