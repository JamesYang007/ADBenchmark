#include <benchmark/benchmark.h>
#include <iostream>
#include <fastad>
#include <util/check_gradient.hpp>

namespace adb {

template <class F>
void BM_fastad(benchmark::State& state)
{
    F f;
    size_t N = state.range(0);

    Eigen::VectorXd x(N);
    double fx = 0;
    f.fill(x);
    Eigen::VectorXd grad_fx(x.size());

    ad::VarView<double, ad::vec> x_ad(x.data(), 
                                      grad_fx.data(), 
                                      x.size());

    auto expr = f(x_ad);

    auto size_pack = expr.bind_cache_size();
    Eigen::VectorXd val_buf(size_pack(0));
    Eigen::VectorXd adj_buf(size_pack(1));
    expr.bind_cache({val_buf.data(), adj_buf.data()});

    state.counters["N"] = x.size();

    for (auto _ : state) {
        grad_fx.setZero();
        fx = ad::autodiff(expr);
    }

    // sanity-check that output gradient is good
    Eigen::VectorXd expected(grad_fx.size());
    f.derivative(x, expected);
    check_gradient(grad_fx, expected, "fastad-" + f.name());

#ifdef ADB_FASTAD_BYTESIZE
    size_t vb_size = val_buf.size() * sizeof(double);
    size_t ab_size = adj_buf.size() * sizeof(double);
    size_t e_size = sizeof(expr);

    std::cout << "FastAD val_buf bytesize: "
              << vb_size
              << std::endl;

    std::cout << "FastAD adj_buf bytesize: "
              << ab_size
              << std::endl;

    std::cout << "FastAD expr bytesize: "
              << e_size
              << std::endl;

    std::cout << "FastAD total bytesize: "
              << vb_size + ab_size + e_size
              << std::endl;
#endif

}

} // namespace adb
