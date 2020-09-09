#include <adept/driver.hpp>
#include <functor/sum_iter.hpp>

namespace adb {

struct SumIterFunc: SumIterFuncBase
{
    adept::aReal operator()(const adept::aVector& x) const
    {
        adept::adouble sum_x = 0;
        for (int i = 0; i < x.size(); ++i) {
            sum_x += x(i);
        }
        return sum_x;
    }
};

BENCHMARK_TEMPLATE(BM_adept, SumIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
