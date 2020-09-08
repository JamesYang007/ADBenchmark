#include <fastad/driver.hpp>
#include <functor/prod.hpp>

namespace adb {

struct ProdFunc : ProdFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        return ad::prod(x);
    }
};

BENCHMARK_TEMPLATE(BM_fastad, ProdFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
