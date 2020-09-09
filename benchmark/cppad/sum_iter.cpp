#include <cppad/driver.hpp>
#include <functor/sum_iter.hpp>

namespace adb {

struct SumIterFunc: SumIterFuncBase
{};

BENCHMARK_TEMPLATE(BM_cppad, SumIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
