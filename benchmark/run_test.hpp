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
                           const Eigen::VectorXd& expected,
                           const std::string& name)
{
    constexpr double eps = 1e-15;
    auto diff = (actual.array() - expected.array()).abs();
    if ((actual.array() != expected.array()).any()) {
        std::cerr << "WARNING (" << name << "): result not identical" << std::endl;
        std::cerr << "Max abs diff: " << diff.maxCoeff() << std::endl;
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
    grad_fx.setZero();
    double fx = 0;
    StopWatch<> sw;

    // save gradient to compare with others
    Eigen::VectorXd expected(N);
    f.derivative(x, expected);

    os << N;

    //// Adept
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    adept_gradient(f, x, fx, grad_fx);
    //}
    //sw.stop();
    //check_gradient(grad_fx, expected, "adept");
    //os << ',' << sw.elapsed() / n_iter;

    //// ADOLC
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    adolc_gradient(f, x, fx, grad_fx);
    //}
    //sw.stop();
    //check_gradient(grad_fx, expected, "adolc");
    //os << ',' << sw.elapsed() / n_iter;

    //// CppAD
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    cppad_gradient(f, x, fx, grad_fx);
    //}
    //sw.stop();
    //check_gradient(grad_fx, expected, "cppad");
    //os << ',' << sw.elapsed() / n_iter;

    //// Sacado
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    sacado_gradient(f, x, fx, grad_fx);
    //}
    //sw.stop();
    //check_gradient(grad_fx, expected, "sacado");
    //os << ',' << sw.elapsed() / n_iter;

    // STAN
    sw.start();
    for (int i = 0; i < n_iter; ++i) {
        stan::math::gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    check_gradient(grad_fx, expected, "stan");
    os << ',' << sw.elapsed() / n_iter;

    //// FastAD
    //Eigen::VectorXd val_buf;
    //Eigen::VectorXd adj_buf;
    //ad::VarView<double, ad::vec> fastad_x_ad(x.data(),
    //                                         grad_fx.data(),
    //                                         x.size());
    //auto expr = f(fastad_x_ad);
    //auto size_pack = expr.bind_cache_size();
    //val_buf.resize(size_pack(0));
    //adj_buf.resize(size_pack(1));
    //expr.bind_cache({val_buf.data(), adj_buf.data()});
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    fastad_gradient(expr, fx, grad_fx);
    //}
    //sw.stop();
    //check_gradient(grad_fx, expected, "fastad");
    //os << ',' << sw.elapsed() / n_iter;

    //// double (baseline)
    //sw.start();
    //for (int i = 0; i < n_iter; ++i) {
    //    double_gradient(f, x, fx, grad_fx);
    //}
    //sw.stop();
    //std::cout << "(print double value to prevent-inline): " << fx << std::endl;
    //os << ',' << sw.elapsed() / n_iter;

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
