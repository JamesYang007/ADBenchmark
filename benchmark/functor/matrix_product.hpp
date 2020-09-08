#pragma once
#include <string>
#include <Eigen/Dense>
//#include <adept.h>
//#include <adept_arrays.h>
//#include <stan/math.hpp>

namespace adb {

struct MatrixProductFuncBase
{
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
        int N = static_cast<int>(sqrt(x.size() / 2));
        if (N < 1) N = 1;
        x.resize(N * N * 2);
        x = Eigen::VectorXd::LinSpaced(1, 1, x.size()) / (x.size() + 1.);
    }
};

//struct MatrixProductFunc
//{
//    template <class T>
//    T operator()(Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
//    {
//        using mat_t = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
//        size_t N = std::sqrt(x.size() / 2);
//        Eigen::Map<mat_t> x1(x.data(), N, N);
//        Eigen::Map<mat_t> x2(x.data() + x.size()/2, N, N);
//        return (x1 * x2).array().sum();
//    }
//
//    adept::aReal operator()(adept::aVector& x) const
//    {
//        using namespace adept;
//        size_t N = std::sqrt(x.size() / 2);
//        adept::aMatrix mat1(N, N);
//        adept::aMatrix mat2(N, N);
//        for (int i = 0; i < N; ++i) {
//            for (int j = 0; j < N; ++j) {
//                mat1(i,j) = x[i + N * j];
//                mat2(i,j) = x[x.size() / 2 + i + N * j];
//            }
//        }
//        return adept::sum(adept::matmul(mat1, mat2));
//    }
//
//    auto operator()(Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
//    {
//        using mat_t = Eigen::Matrix<stan::math::var, Eigen::Dynamic, Eigen::Dynamic>;
//        size_t N = std::sqrt(x.size() / 2);
//        Eigen::Map<mat_t> x1(x.data(), N, N);
//        Eigen::Map<mat_t> x2(x.data() + x.size()/2, N, N);
//        return stan::math::sum(stan::math::multiply(x1, x2));
//    }
//
//    auto operator()(const Eigen::VectorXd& x) const
//    {
//        size_t N = std::sqrt(x.size() / 2);
//        Eigen::Map<const Eigen::MatrixXd> x1(x.data(), N, N);
//        Eigen::Map<const Eigen::MatrixXd> x2(x.data() + x.size()/2, N, N);
//        return (x1 * x2).array().sum();
//    }
//};

} // namespace adb
