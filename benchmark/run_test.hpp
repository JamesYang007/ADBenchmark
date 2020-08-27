#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <result.hpp>
#include <stopwatch.hpp>
#include <gradient.hpp>
#include <Eigen/Dense>
#include <adept.h>

namespace adb {

// Benchmark invoking f and return the average time over n_iter iterations.
template <class F>
inline double benchmark(F&& f, size_t n_iter)
{
    StopWatch<> sw;
    double elapsed = 0;
    for (int i = 0; i < n_iter; ++i) {
        sw.start();
        f();
        sw.stop();
        elapsed += sw.elapsed();
    }
    return elapsed / n_iter;
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
    double fx = 0;

    os << N;

    // Adept
    os << ',' << benchmark([&]() { adept_gradient(f, x, fx, grad_fx); }, n_iter);

    // FastAD
    os << ',' << benchmark([&]() { fastad_gradient(f, x, fx, grad_fx); }, n_iter);

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
