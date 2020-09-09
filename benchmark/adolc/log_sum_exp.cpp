#include <adolc/driver.hpp>
#include <functor/log_sum_exp.hpp>

namespace adb {

struct LogSumExpFunc: LogSumExpFuncBase
{};

BENCHMARK_TEMPLATE(BM_adolc, LogSumExpFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
