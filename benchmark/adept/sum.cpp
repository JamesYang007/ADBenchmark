#include <adept/driver.hpp>
#include <functor/sum.hpp>

namespace adb {

struct SumFunc: SumFuncBase
{
    adept::aReal operator()(const adept::aVector& x) const
    {
        return adept::sum(x);
    }
};

BENCHMARK_TEMPLATE(BM_adept, SumFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
