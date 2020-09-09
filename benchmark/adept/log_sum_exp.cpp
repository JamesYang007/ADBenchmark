#define ADEPT_FAST_EXPONENTIAL
#include <adept/driver.hpp>
#include <functor/log_sum_exp.hpp>

namespace adb {

struct LogSumExpFunc: LogSumExpFuncBase
{
    adept::aReal operator()(const adept::aVector& x) const
    {
        return adept::log(adept::sum(adept::fastexp(x)));
    }
};

BENCHMARK_TEMPLATE(BM_adept, LogSumExpFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
