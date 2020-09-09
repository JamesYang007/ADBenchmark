#include <adolc/driver.hpp>
#include <functor/sum_iter.hpp>

namespace adb {

struct SumIterFunc: SumIterFuncBase
{};

BENCHMARK_TEMPLATE(BM_adolc, SumIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
