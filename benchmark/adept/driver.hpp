#include <benchmark/benchmark.h>
#include <adept.h>
#include <adept_arrays.h>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
static void BM_adept(benchmark::State& state)
{
    // Stack can be reused in multiple iterations
    adept::Stack stack;

    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    f.fill(x);
    double fx;
    Eigen::VectorXd grad_fx(x.size());

    adept::aVector x_ad(x.size());

    state.counters["N"] = x.size();

    for (auto _ : state) {
        for (int i = 0; i < x.size(); ++i) {
            x_ad[i].set_value(x(i));
        }
        adept::active_stack()->new_recording();
        adept::aReal fx_ad = f(x_ad);
        fx = fx_ad.value();
        fx_ad.set_gradient(1.0);
        adept::active_stack()->compute_adjoint();
        for (int i = 0; i < x_ad.size(); ++i) {
            grad_fx(i) = x_ad[i].get_gradient();
        }
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "adept-" + f.name());
}

} // namespace adb
