#include <adept/driver.hpp>
#include <functor/matrix_product.hpp>

namespace adb {

struct MatrixProductFunc: MatrixProductFuncBase
{
    adept::aReal operator()(adept::aVector& x) const
    {
        size_t N = std::sqrt(x.size() / 2);
        adept::aMatrix mat1(N, N);
        adept::aMatrix mat2(N, N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                mat1(i,j) = x[i + N * j];
                mat2(i,j) = x[x.size() / 2 + i + N * j];
            }
        }
        return adept::sum(adept::matmul(mat1, mat2));
    }
};

BENCHMARK_TEMPLATE(BM_adept, MatrixProductFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 16);

} // namespace adb
