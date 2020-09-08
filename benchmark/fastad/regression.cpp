#include <fastad/driver.hpp>
#include <functor/regression.hpp>

namespace adb {

struct RegressionFunc: RegressionFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = (x.size() - 2);
        ad::VarView<T, ad::vec> w(x.data(), x.data_adj(), N);
        ad::VarView<T> b(x.data() + N, x.data_adj() + N);
        ad::VarView<T> sigma(x.data() + N + 1, x.data_adj() + N + 1);
        return ad::normal_adj_log_pdf(y, ad::dot(X, w) + b, sigma) 
                + ad::normal_adj_log_pdf(w, 0., 1.)
                + ad::normal_adj_log_pdf(b, 0., 1.)
                + ad::uniform_adj_log_pdf(sigma, 0.1, 10.);
    }
};

BENCHMARK_TEMPLATE(BM_fastad, RegressionFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
