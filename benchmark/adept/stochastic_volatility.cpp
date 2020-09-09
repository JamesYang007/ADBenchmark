#define ADEPT_FAST_EXPONENTIAL
#include <adept/driver.hpp>
#include <functor/stochastic_volatility.hpp>

namespace adb {

struct StochasticVolatilityFunc: StochasticVolatilityFuncBase
{
    adept::aReal operator()(const adept::aVector& x) 
    {
        size_t N = (x.size() - 3) / 2;
        adept::aVector h_std = x(adept::range(0, N-1));
        adept::aVector h = x(adept::range(N, 2*N - 1));
        adept::aReal phi = x(2*N);
        adept::aReal sigma = x(2*N+1);
        adept::aReal mu = x(2*N+2);

        h = h_std * sigma;
        h[0] /= adept::sqrt(1. - phi * phi);
        h += mu;
        for (size_t i = 1; i < N; ++i) {
            h[i] += phi * (h[i-1] - mu);
        }

        adept::Vector aY(y.data(), adept::dimensions(y.size()));
        adept::aVector y_stddev = adept::fastexp(h/2);
        return normal_log_density(aY, 0., y_stddev) +
                normal_log_density(h_std, 0., 1.) +
                cauchy_log_density(sigma, 0., 5.) +
                cauchy_log_density(mu, 0., 10.) -
                std::log(2.);
    }

    adept::aReal normal_log_density(const adept::aVector& y,
                                    double mu,
                                    double sigma) const {
        adept::aReal z = adept::norm2(y - mu) / sigma;
        return -0.5 * z * z - y.size() * std::log(sigma);
    }

    adept::aReal normal_log_density(const adept::aVector& y,
                                    double mu,
                                    const adept::aVector& sigma) const {
        adept::aReal z = adept::norm2((y - mu) / sigma);
        return -0.5 * z * z - adept::sum(adept::log(sigma));
    }
};

BENCHMARK_TEMPLATE(BM_adept, StochasticVolatilityFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb

