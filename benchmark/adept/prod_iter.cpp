#include <adept/driver.hpp>
#include <functor/prod_iter.hpp>

namespace adb {

struct ProdIterFunc: ProdIterFuncBase
{
    adept::aReal operator()(const adept::aVector& x) const
    {
        adept::aReal product_x = 1;
        for (int i = 0; i < x.size(); ++i) {
            product_x *= x(i);
        }
        return product_x;
    }
};

BENCHMARK_TEMPLATE(BM_adept, ProdIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
