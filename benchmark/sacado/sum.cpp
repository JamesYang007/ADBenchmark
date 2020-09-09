#include <sacado/driver.hpp>
#include <functor/sum.hpp>

namespace adb {

struct SumFunc: SumFuncBase
{};

BENCHMARK_TEMPLATE(BM_sacado, SumFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
