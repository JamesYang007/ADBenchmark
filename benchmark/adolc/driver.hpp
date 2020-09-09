#include <array>
#include <benchmark/benchmark.h>
#include <adolc/adolc.h>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
static void BM_adolc(benchmark::State& state)
{
    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    f.fill(x);
    double fx;
    Eigen::VectorXd grad_fx(x.size());

    Eigen::Matrix<adouble, Eigen::Dynamic, 1> x_ad(x.size());
    trace_on(1);
    for (int n = 0; n < x.size(); ++n) {
        x_ad(n) <<= x(n);
    }
    adouble y = f(x_ad);
    y >>= fx;
    trace_off();

    std::array<double, 1> u = {1};

    state.counters["N"] = x.size();

    for (auto _ : state) {
        zos_forward(1, 1, x.size(), 1, x.data(), &fx);
        fos_reverse(1, 1, x.size(), u.data(), grad_fx.data());
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "adolc-" + f.name());
}

} // namespace adb
