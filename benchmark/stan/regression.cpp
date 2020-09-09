#include <stan/driver.hpp>
#include <functor/regression.hpp>

namespace adb {

struct RegressionFunc: RegressionFuncBase
{
    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        using namespace stan::math;
        using vec_t = Eigen::Matrix<var, Eigen::Dynamic, 1>;
        size_t N = (x.size() - 2);
        Eigen::Map<const vec_t> w(x.data(), N);
        auto& b = x(N);
        auto& sigma = x(N + 1);
        return normal_lpdf(y, multiply(X, w) + b * vec_t::Ones(1000), sigma) +
                normal_lpdf(w, 0., 1.) +
                normal_lpdf(b, 0., 1.) -
                uniform_lpdf(sigma, 0.1, 10.);
    }
};

BENCHMARK_TEMPLATE(BM_stan, RegressionFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
