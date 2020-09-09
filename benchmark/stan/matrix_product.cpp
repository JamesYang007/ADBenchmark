#include <stan/driver.hpp>
#include <functor/matrix_product.hpp>

namespace adb {

struct MatrixProductFunc: MatrixProductFuncBase
{
    stan::math::var operator()(const Eigen::Matrix<stan::math::var, Eigen::Dynamic, 1>& x) const
    {
        using mat_t = Eigen::Matrix<stan::math::var, Eigen::Dynamic, Eigen::Dynamic>;
        size_t N = std::sqrt(x.size() / 2);
        Eigen::Map<const mat_t> x1(x.data(), N, N);
        Eigen::Map<const mat_t> x2(x.data() + x.size()/2, N, N);
        return stan::math::sum(stan::math::multiply(x1, x2));
    }
};

BENCHMARK_TEMPLATE(BM_stan, MatrixProductFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 16);

} // namespace adb
