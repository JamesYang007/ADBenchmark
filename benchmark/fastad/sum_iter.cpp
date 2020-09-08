#include <util/counting_iterator.hpp>
#include <fastad/driver.hpp>
#include <functor/sum_iter.hpp>

namespace adb {

struct SumIterFunc: SumIterFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        return ad::sum(counting_iterator<>(0),
                       counting_iterator<>(x.size()),
                       [&](size_t i) { return x[i]; });
    }
};

BENCHMARK_TEMPLATE(BM_fastad, SumIterFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
