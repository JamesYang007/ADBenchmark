#include <adept/driver.hpp>
#include <functor/regression.hpp>

namespace adb {

struct RegressionFunc: RegressionFuncBase
{
    using base_t = RegressionFuncBase;

    adept::aReal operator()(const adept::aVector& x) 
    {
        size_t N = (x.size() - 2);
        adept::aVector w = x(adept::range(0, N-1));
        adept::aReal b = x[N];
        adept::aReal sigma = x[N+1];
        adept::Matrix aX(X_t.data(), adept::dimensions(X.rows(), X.cols()));
        adept::Vector aY(y.data(), adept::dimensions(y.size()));
        adept::aVector z = aY - (adept::matmul(aX, w) + b);
        return normal_log_density(z, 0., sigma) +
                normal_log_density(w, 0., 1.) +
                normal_log_density(b, 0., 1.) -
                std::log(10. - 0.1);
    }

    template <class T, class S>
    adept::aReal normal_log_density(const adept::aVector& y,
                                    T mu,
                                    S sigma) 
    {
        using std::log;
        using adept::log;
        adept::aReal z = adept::norm2(y - mu) / sigma;
        return -0.5 * z * z - y.size() * log(sigma);
    }

    adept::aReal normal_log_density(const adept::aReal& y,
                                    double mu,
                                    double sigma) 
    {
        adept::aReal z = (y - mu) / sigma;
        return -0.5 * z * z - std::log(sigma);
    }

    // Adept is more efficient with row-major matrix
    // Create a transpose matrix of X and use this to
    // create adept::Matrix in operator()
    void fill(Eigen::VectorXd& x)
    {
        base_t::fill(x);
        X_t = base_t::X.transpose();
    }

private:
    Eigen::MatrixXd X_t;
};

BENCHMARK_TEMPLATE(BM_adept, RegressionFunc)
    -> RangeMultiplier(2) -> Range(1, 1 << 14);

} // namespace adb
