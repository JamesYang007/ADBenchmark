#include <fastad/driver.hpp>
#include <functor/normal_log_pdf.hpp>

namespace adb {

struct NormalLogPdfFunc: NormalLogPdfFuncBase
{
    template <class T>
    auto operator()(const ad::VarView<T, ad::vec>& x) const
    {
        return ad::normal_adj_log_pdf(x, mu_, sigma_);
    }
};

BENCHMARK_TEMPLATE(BM_fastad, NormalLogPdfFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
