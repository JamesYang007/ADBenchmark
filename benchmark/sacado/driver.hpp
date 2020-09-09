#include <array>
#include <benchmark/benchmark.h>
#include <Sacado_No_Kokkos.hpp>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
static void BM_sacado(benchmark::State& state)
{
    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    f.fill(x);
    double fx;
    Eigen::VectorXd grad_fx(x.size());

    Eigen::Matrix<Sacado::Rad::ADvar<double>, Eigen::Dynamic, 1> x_ad(x.size());

    state.counters["N"] = x.size();

    for (auto _ : state) {
        for (int n = 0; n < x.size(); ++n) {
            x_ad(n) = x[n];
        }
        fx = f(x_ad).val();
        Sacado::Rad::ADvar<double>::Gradcomp();
        for (int n = 0; n < x.size(); ++n) {
            grad_fx(n) = x_ad(n).adj();
        }
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "sacado-" + f.name());
}

} // namespace adb
