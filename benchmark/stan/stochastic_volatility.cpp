#include <stan/driver.hpp>
#include <functor/stochastic_volatility.hpp>

namespace adb {

struct StochasticVolatilityFunc: StochasticVolatilityFuncBase
{
    auto operator()(Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        using namespace stan::math;
        using vec_t = Eigen::Matrix<var, Eigen::Dynamic, 1>;
        size_t N = (x.size() - 3) / 2;
        Eigen::Map<vec_t> h_std(x.data(), N);
        Eigen::Map<vec_t> h(x.data() + N, N);
        auto& phi = x(2*N);
        auto& sigma = x(2*N + 1);
        auto& mu = x(2*N + 2);
        h = h_std * sigma;
        h[0] /= sqrt(1. - phi * phi);
        h += mu * Eigen::VectorXd::Ones(N);
        for (size_t i = 1; i < N; ++i) {
            h[i] += phi * (h[i-1] - mu);
        }
        auto lp = normal_lpdf(y, 0., exp(h / 2.)) +
                normal_lpdf(h_std, 0., 1.) +
                cauchy_lpdf(sigma, 0., 5.) +
                cauchy_lpdf(mu, 0., 10.) +
                uniform_lpdf(phi, -1., 1.);

#ifdef STOCHASTIC_VOLATILITY_BYTESIZE

        auto stack = stan::math::ChainableStack::instance_;

        size_t vs_size = stack->var_stack_.size() *
                         sizeof(std::decay_t<decltype(stack->var_stack_[0])>);
        size_t vns_size = stack->var_nochain_stack_.size() *
                          sizeof(std::decay_t<decltype(stack->var_nochain_stack_[0])>);
        size_t vas_size = stack->var_alloc_stack_.size() *
                          sizeof(std::decay_t<decltype(stack->var_alloc_stack_[0])>);
        size_t malloc_size = stack->memalloc_.bytes_allocated();

        std::cout << "STAN var_stack_ bytesize: "
                  << vs_size
                  << std::endl;

        std::cout << "STAN var_nochain_stack_ bytesize: "
                  << vns_size
                  << std::endl;

        std::cout << "STAN var_alloc_stack_ bytesize: "
                  << vas_size
                  << std::endl;

        std::cout << "STAN memalloc_.bytes_allocated bytesize: "
                  << malloc_size
                  << std::endl;

        std::cout << "STAN lower bound bytesize: "
                  << vs_size + vns_size + vas_size + malloc_size
                  << std::endl;

#endif
        return lp;
    }
};

BENCHMARK_TEMPLATE(BM_stan, StochasticVolatilityFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb

