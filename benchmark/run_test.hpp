#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <result.hpp>
#include <stopwatch.hpp>
#include <gradient.hpp>
#include <Eigen/Dense>
#include <stan/math.hpp>
#include <adept.h>

namespace adb {

inline void check_gradient(const Eigen::VectorXd& actual,
                           const Eigen::VectorXd& expected)
{
    if (!(actual.array() == expected.array()).all()) {
        std::cerr << "WARNING: result not identitcal" << std::endl;
    }
}

// Computes average time for differentiating f for each library
template <class F>
inline void time_gradients(const F& f, 
                           Eigen::VectorXd& x,
                           size_t n_iter,
                           std::ostream& os) 
{
    int N = x.size();
    Eigen::VectorXd grad_fx(N);
    Eigen::VectorXd expected(N);
    grad_fx.setZero();
    double fx = 0;
    StopWatch<> sw;

    os << N;

    // Adept
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        adept_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    os << ',' << sw.elapsed() / n_iter;

    expected = grad_fx;  // save gradient to compare with others

    // ADOLC
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        adolc_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected);
    os << ',' << sw.elapsed() / n_iter;

    // CppAD
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        cppad_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected);
    os << ',' << sw.elapsed() / n_iter;

    // Sacado
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        sacado_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected);
    os << ',' << sw.elapsed() / n_iter;

    // STAN
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        stan::math::gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected);
    os << ',' << sw.elapsed() / n_iter;

    // FastAD
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        fastad_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected);
    os << ',' << sw.elapsed() / n_iter;

    // double (baseline)
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        double_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    std::cout << "(print double value to prevent-inline): " << fx << std::endl;
    os << ',' << sw.elapsed() / n_iter;

    os << '\n';
}

// Benchmarks differentiating f for various AD libraries.
// For each library, f is differentiated n_iter number of times
// and the average time is taken.
// It benchmarks for input sizes in the set 2^[0, ..., 14].
template <class F>
inline void run_test(F f, size_t n_iter) 
{
    adept::Stack stack;
    std::string file_name = f.name() + "_eval.csv";
    std::fstream fs(file_name, std::fstream::out);
    print_results_header(fs);
    constexpr int max = 16 * 1024;
    for (int N = 1; N <= max; N *= 2) {
        std::cout << "N = " << N << std::endl;
        Eigen::VectorXd x(N);
        f.fill(x);
        time_gradients(f, x, n_iter, fs);
    }
}
    
} // namespace adb
