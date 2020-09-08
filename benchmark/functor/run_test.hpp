#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <gradient.hpp>
#include <adolc/adouble.h>
#include <adolc/taping.h>
#include <stan/math/rev/functor/gradient.hpp>
#include <adept.h>
#include <testpack.hpp>

namespace adb {

// Computes average time for differentiating f for each library
template <class F>
inline void time_gradients(const F& f, 
                           Eigen::VectorXd& x,
                           std::unordered_map<adb::TestName, adb::TestPack>& packs,
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

    // Adept
    auto& adept_pack = packs.at(adb::TestName::adept);
    if (adept_pack.run) {
        adept::aVector x_ad(x.size());
        sw.start();
        for (int i = 0; i < adept_pack.n_iter; ++i) {
            adept_gradient(f, x, x_ad, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, adept_pack.name);
        adept_pack.time = sw.elapsed() / adept_pack.n_iter;
    }

    // ADOLC
    auto& adolc_pack = packs.at(adb::TestName::adolc);
    if (adolc_pack.run) {
        Eigen::Matrix<adouble, Eigen::Dynamic, 1> x_ad(x.size());
        trace_on(1);
        for (int n = 0; n < x.size(); ++n) {
            x_ad(n) <<= x(n);
        }
        adouble y = f(x_ad);
        y >>= fx;
        trace_off();
        sw.start();
        for (int i = 0; i < adolc_pack.n_iter; ++i) {
            adolc_gradient(x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, adolc_pack.name);
        adolc_pack.time = sw.elapsed() / adolc_pack.n_iter;
    }

    // CppAD
    auto& cppad_pack = packs.at(adb::TestName::cppad);
    if (cppad_pack.run) {
        Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> x_ad(x.size());
        CppAD::Independent(x_ad);
        Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> y(1);
        y[0] = f(x_ad);
        CppAD::ADFun<double> g(x_ad, y);
        Eigen::VectorXd w(1);
        w(0) = 1.;
        sw.start();
        for (int i = 0; i < cppad_pack.n_iter; ++i) {
            cppad_gradient(w, g, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, cppad_pack.name);
        cppad_pack.time = sw.elapsed() / cppad_pack.n_iter;
    }

    // Sacado
    auto& sacado_pack = packs.at(adb::TestName::sacado);
    if (sacado_pack.run) {
        Eigen::Matrix<Sacado::Rad::ADvar<double>, Eigen::Dynamic, 1> x_ad(x.size());
        sw.start();
        for (int i = 0; i < sacado_pack.n_iter; ++i) {
            sacado_gradient(f, x, x_ad, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, sacado_pack.name);
        sacado_pack.time = sw.elapsed() / sacado_pack.n_iter;
    }

    // double (baseline)
    size_t double_iter = 10000;
    sw.start();
    for (int i = 0; i < double_iter; ++i) {
        double_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    std::cout << "(print double value to prevent-inline): " << fx << std::endl;

    // print the timings by iterating the unordered_map
    print_results(os, packs);

    os << ',' << sw.elapsed() / double_iter;
    os << '\n';
}

// Benchmarks differentiating f for various AD libraries.
// For each library, f is differentiated n_iter number of times
// and the average time is taken.
// It benchmarks for input sizes in the set 2^[0, ..., 14].
template <class F>
inline void run_test(F f, 
                     std::unordered_map<adb::TestName, adb::TestPack>& packs,
                     size_t max = 16 * 1024) 
{
    adept::Stack stack;
    std::string file_name = f.name() + "_eval.csv";
    std::fstream fs(file_name, std::fstream::out);
    print_results_header(fs, packs);
    for (int N = 1; N <= max; N *= 2) {
        std::cout << "N = " << N << std::endl;
        Eigen::VectorXd x(N);
        f.fill(x);
        time_gradients(f, x, packs, fs);
    }
}
    
} // namespace adb
