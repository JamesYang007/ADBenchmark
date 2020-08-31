#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <result.hpp>
#include <stopwatch.hpp>
#include <gradient.hpp>
#include <Eigen/Dense>
#include <stan/math.hpp>
#include <adept.h>
#include <testpack.hpp>

namespace adb {

inline void check_gradient(const Eigen::VectorXd& actual,
                           const Eigen::VectorXd& expected,
                           const std::string& name)
{
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
        sw.start();
        for (int i = 0; i < adept_pack.n_iter; ++i) {
            adept_gradient(f, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, adept_pack.name);
        adept_pack.time = sw.elapsed() / adept_pack.n_iter;
    }

    // ADOLC
    auto& adolc_pack = packs.at(adb::TestName::adolc);
    if (adolc_pack.run) {
        sw.start();
        for (int i = 0; i < adolc_pack.n_iter; ++i) {
            adolc_gradient(f, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, adolc_pack.name);
        adolc_pack.time = sw.elapsed() / adolc_pack.n_iter;
    }

    // CppAD
    auto& cppad_pack = packs.at(adb::TestName::cppad);
    if (cppad_pack.run) {
        sw.start();
        for (int i = 0; i < cppad_pack.n_iter; ++i) {
            cppad_gradient(f, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, cppad_pack.name);
        cppad_pack.time = sw.elapsed() / cppad_pack.n_iter;
    }

    // Sacado
    auto& sacado_pack = packs.at(adb::TestName::sacado);
    if (sacado_pack.run) {
        sw.start();
        for (int i = 0; i < sacado_pack.n_iter; ++i) {
            sacado_gradient(f, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, sacado_pack.name);
        sacado_pack.time = sw.elapsed() / sacado_pack.n_iter;
    }

    // STAN
    auto& stan_pack = packs.at(adb::TestName::stan);
    if (stan_pack.run) {
        sw.start();
        for (int i = 0; i < stan_pack.n_iter; ++i) {
            stan::math::gradient(f, x, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, stan_pack.name);
        stan_pack.time = sw.elapsed() / stan_pack.n_iter;
    }

    // FastAD
    auto& fastad_pack = packs.at(adb::TestName::fastad);
    if (fastad_pack.run) {
        Eigen::VectorXd val_buf;
        Eigen::VectorXd adj_buf;
        ad::VarView<double, ad::vec> fastad_x_ad(x.data(),
                                                 grad_fx.data(),
                                                 x.size());
        auto expr = f(fastad_x_ad);
        auto size_pack = expr.bind_cache_size();
        val_buf.resize(size_pack(0));
        adj_buf.resize(size_pack(1));
        expr.bind_cache({val_buf.data(), adj_buf.data()});
        sw.start();
        for (int i = 0; i < fastad_pack.n_iter; ++i) {
            fastad_gradient(expr, fx, grad_fx);
        }
        sw.stop();
        check_gradient(grad_fx, expected, fastad_pack.name);
        fastad_pack.time = sw.elapsed() / fastad_pack.n_iter;
    }

    // double (baseline)
    sw.start();
    for (int i = 0; i < 10000; ++i) {
        double_gradient(f, x, fx, grad_fx);
    }
    sw.stop();
    std::cout << "(print double value to prevent-inline): " << fx << std::endl;

    // print the timings by iterating the unordered_map
    print_results(os, packs);

    os << ',' << sw.elapsed() / 10000;
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
