#include <stan/driver.hpp>
#include <functor/normal_log_pdf.hpp>

namespace adb {

struct NormalLogPdfFunc: NormalLogPdfFuncBase
{
    auto operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        stan::math::var mu = mu_;
        stan::math::var sigma = sigma_;
        return stan::math::normal_lpdf(x, mu, sigma);
    }
};

BENCHMARK_TEMPLATE(BM_stan, NormalLogPdfFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
