#include <fastad/driver.hpp>
#include <functor/sum.hpp>

namespace adb {

struct SumFunc : SumFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        return ad::sum(x);
    }
};

BENCHMARK_TEMPLATE(BM_fastad, SumFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
