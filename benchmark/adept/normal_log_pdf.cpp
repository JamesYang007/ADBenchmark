#include <adept/driver.hpp>
#include <functor/normal_log_pdf.hpp>

namespace adb {

struct NormalLogPdfFunc: NormalLogPdfFuncBase
{
    adept::aReal operator()(const adept::aVector& x) const
    {
        double neg_log_sigma = -std::log(sigma_);
        adept::aReal z = adept::norm2(x - mu_) / sigma_;
        return -0.5 * z * z - x.size() * neg_log_sigma;
    }
};

BENCHMARK_TEMPLATE(BM_adept, NormalLogPdfFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
