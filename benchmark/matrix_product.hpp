#pragma once
#include <string>
#include <counting_iterator.hpp>
#include <Eigen/Dense>
#include <adept.h>
#include <adept_arrays.h>
#include <stan/math.hpp>
#include <fastad>

namespace adb {

struct MatrixProductFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        int N = static_cast<int>(std::sqrt(x.size() / 2));
        using Eigen::Dynamic;
        using Eigen::Matrix;
        Matrix<T, Dynamic, Dynamic> a(N,N);
        Matrix<T, Dynamic, Dynamic> b(N,N);

        int i = 0;
        for (int m = 0; m < N; ++m) {
            for (int n = 0; n < N; ++n) {
                a(m,n) = x(m + n * N);
                b(m,n) = x(x.size() / 2 + m + n * N);
            }
        }

        Matrix<T, Dynamic, Dynamic> ab(N,N);
        for (int m = 0; m < N; ++m) {
            for (int n = 0; n < N; ++n) {
                ab(m,n) = 0;
                for (int k = 0; k < N; ++k) {
                    ab(m,n) += a(m,k) * b(k,n);
                }
            }
        }

        T sum = 0;
        for (int m = 0; m < N; ++m) {
            for (int n = 0; n < N; ++n) {
                sum += ab(m,n);
            }
        }
        return sum;
    }

    adept::aReal operator()(adept::aVector& x) const
    {
        size_t N = std::sqrt(x.size() / 2);
        adept::aMatrix mat1(N, N);
        adept::aMatrix mat2(N, N);
        int k = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j, ++k) {
                mat1(i,j) = x[k];
                mat1(i,j) = x[x.size() / 2 + k];
            }
        }
        return adept::sum(adept::matmul(mat1, mat2));
    }

    auto operator()(Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        using mat_t = Eigen::Matrix<stan::math::var, Eigen::Dynamic, Eigen::Dynamic>;
        size_t N = std::sqrt(x.size() / 2);
        Eigen::Map<mat_t> x1(x.data(), N, N);
        Eigen::Map<mat_t> x2(x.data() + x.size()/2, N, N);
        return stan::math::sum(stan::math::multiply(x1, x2));
    }

    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = std::sqrt(x.size() / 2);
        ad::VarView<T, ad::mat> m1(x.data(),
                                   x.data_adj(),
                                   N, N);
        ad::VarView<T, ad::mat> m2(x.data() + x.size()/2, 
                                   x.data_adj() + x.size()/2,
                                   N, N);
        return ad::sum(ad::dot(m1, m2));
    }

    auto operator()(const Eigen::VectorXd& x) const
    {
        size_t N = std::sqrt(x.size() / 2);
        Eigen::Map<const Eigen::MatrixXd> x1(x.data(), N, N);
        Eigen::Map<const Eigen::MatrixXd> x2(x.data() + x.size()/2, N, N);
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

    void fill(Eigen::VectorXd& x) {
        int N = static_cast<int>(sqrt(x.size() / 2));
        if (N < 1) N = 1;
        x.resize(N * N * 2);
        for (int i = 0; i < x.size(); ++i) {
            x(i) = static_cast<double>(i + 1) / (x.size() + 1);
        }
    }
};

} // namespace adb
