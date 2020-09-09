#include <adolc/driver.hpp>
#include <functor/matrix_product.hpp>

namespace adb {

struct MatrixProductFunc: MatrixProductFuncBase
{};

BENCHMARK_TEMPLATE(BM_adolc, MatrixProductFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 16);

} // namespace adb
