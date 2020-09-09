#pragma once
#include <string>
#include <functor/functor_base.hpp>

namespace adb {

struct MatrixProductFuncBase: FuncBase
{
    using base_t = FuncBase;

    template <class T>
    T operator()(Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        using mat_t = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
        size_t N = std::sqrt(x.size() / 2);
        Eigen::Map<const mat_t> x1(x.data(), N, N);
        Eigen::Map<const mat_t> x2(x.data() + x.size()/2, N, N);
        return (x1 * x2).array().sum();
    }

    void derivative(const Eigen::VectorXd& x,
                    Eigen::VectorXd& grad) const 
    {
        size_t N = std::sqrt(x.size() / 2);
        Eigen::Map<const Eigen::MatrixXd> x1(x.data(), N, N);
        Eigen::Map<const Eigen::MatrixXd> x2(x.data() + x.size()/2, N, N);
        Eigen::Map<Eigen::MatrixXd> dx1(grad.data(), N, N);
        Eigen::Map<Eigen::MatrixXd> dx2(grad.data() + x.size()/2, N, N);
        dx1 = Eigen::MatrixXd::Ones(N,N) * x2.transpose();
        dx2 = x1.transpose() * Eigen::MatrixXd::Ones(N,N);
    }

    std::string name() const { return "matrix_product"; }

    void fill(Eigen::VectorXd& x) const {
        size_t N = static_cast<size_t>(std::sqrt(x.size() / 2));
        if (N < 1) N = 1;
        x.resize(N * N * 2);
        base_t::fill(x);
    }
};

} // namespace adb
