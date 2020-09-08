#include <fastad/driver.hpp>
#include <util/counting_iterator.hpp>
#include <functor/stochastic_volatility.hpp>

namespace adb {

struct StochasticVolatilityFunc: StochasticVolatilityFuncBase
{
    template <class T>
    auto operator()(ad::VarView<T, ad::vec>& x) const
    {
        size_t N = (x.size() - 3) / 2;
        ad::VarView<T, ad::vec> h_std(x.data(), x.data_adj(), N);
        ad::VarView<T, ad::vec> h(x.data() + N, x.data_adj() + N, N);
        ad::VarView<T> phi(x.data() + 2*N, x.data_adj() + 2*N);
        ad::VarView<T> sigma(x.data() + 2*N + 1, x.data_adj() + 2*N + 1);
        ad::VarView<T> mu(x.data() + 2*N + 2, x.data_adj() + 2*N + 2);
        auto tp = (
            h = h_std * sigma,
            h[0] /= ad::sqrt(1. - phi * phi),
            h += mu,
            ad::for_each(counting_iterator<>(1),
                         counting_iterator<>(N),
                         [&](size_t i) { return h[i] += phi * (h[i-1] - mu); })
        );
        auto lp = ad::normal_adj_log_pdf(y, 0., ad::exp(h / 2.)) 
                + ad::normal_adj_log_pdf(h_std, 0., 1.)
                + ad::cauchy_adj_log_pdf(sigma, 0., 5.)
                + ad::cauchy_adj_log_pdf(mu, 0., 10.) 
                + ad::uniform_adj_log_pdf(phi, -1., 1.);
        return (tp, lp);
    }
};

BENCHMARK_TEMPLATE(BM_fastad, StochasticVolatilityFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
