#include <util/counting_iterator.hpp>
#include <fastad/driver.hpp>
#include <functor/prod_iter.hpp>

namespace adb {

struct ProdIterFunc: ProdIterFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        return ad::prod(counting_iterator<>(0),
                        counting_iterator<>(x.size()),
                        [&](size_t i) { return x[i]; });
   }
};

BENCHMARK_TEMPLATE(BM_fastad, ProdIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
