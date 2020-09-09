#include <baseline/driver.hpp>
#include <functor/prod.hpp>

namespace adb {

struct ProdFunc: ProdFuncBase
{};

BENCHMARK_TEMPLATE(BM_baseline, ProdFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
