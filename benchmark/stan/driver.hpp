#include <benchmark/benchmark.h>
#include <stan/math.hpp>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
static void BM_stan(benchmark::State& state)
{
    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    f.fill(x);
    double fx;
    Eigen::VectorXd grad_fx(x.size());

    state.counters["N"] = x.size();

    for (auto _ : state) {
        stan::math::gradient(f, x, fx, grad_fx);
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "stan-" + f.name());
}

} // namespace adb
