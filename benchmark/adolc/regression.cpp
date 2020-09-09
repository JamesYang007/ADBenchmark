#include <adolc/driver.hpp>
#include <functor/regression.hpp>

namespace adb {

struct RegressionFunc: RegressionFuncBase
{};

BENCHMARK_TEMPLATE(BM_adolc, RegressionFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
