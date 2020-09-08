#include <fastad/driver.hpp>
#include <functor/matrix_product.hpp>

namespace adb {

struct MatrixProductFunc: MatrixProductFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = std::sqrt(x.size() / 2);
        ad::VarView<T, ad::mat> m1(x.data(),
                                   x.data_adj(),
                                   N, N);
        ad::VarView<T, ad::mat> m2(x.data() + x.size()/2, 
                                   x.data_adj() + x.size()/2,
                                   N, N);
        return ad::sum(ad::dot(m1, m2));
    }
};

BENCHMARK_TEMPLATE(BM_fastad, MatrixProductFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 16);

} // namespace adb
