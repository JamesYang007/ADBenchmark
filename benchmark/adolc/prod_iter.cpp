#include <adolc/driver.hpp>
#include <functor/prod_iter.hpp>

namespace adb {

struct ProdIterFunc: ProdIterFuncBase
{};

BENCHMARK_TEMPLATE(BM_adolc, ProdIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
