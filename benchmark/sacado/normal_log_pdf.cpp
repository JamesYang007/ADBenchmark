#include <sacado/driver.hpp>
#include <functor/normal_log_pdf.hpp>

namespace adb {

struct NormalLogPdfFunc: NormalLogPdfFuncBase
{};

BENCHMARK_TEMPLATE(BM_sacado, NormalLogPdfFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
