#include <array>
#include <benchmark/benchmark.h>
#include <cppad/cppad.hpp>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
static void BM_cppad(benchmark::State& state)
{
    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    f.fill(x);
    double fx;
    Eigen::VectorXd grad_fx(x.size());

    Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> x_ad(x.size());
    CppAD::Independent(x_ad);
    Eigen::Matrix<CppAD::AD<double>, Eigen::Dynamic, 1> y(1);
    y[0] = f(x_ad);
    CppAD::ADFun<double> g(x_ad, y);
    Eigen::VectorXd w(1);
    w(0) = 1.;

    state.counters["N"] = x.size();

    for (auto _ : state) {
        fx = g.Forward(0, x)[0];
        grad_fx = g.Reverse(1, w);
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "cppad-" + f.name());
}

} // namespace adb
