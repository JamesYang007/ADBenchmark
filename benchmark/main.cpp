#include <run_test.hpp>
#include <sum.hpp>
#include <sum_iter.hpp>
#include <prod.hpp>
#include <prod_iter.hpp>
#include <log_sum_exp.hpp>
#include <matrix_product.hpp>

int main(int argc, char* argv[])
{
    size_t n_iter = 10000;
    if (argc == 2) {
        n_iter = std::atoi(argv[1]);
    }
    std::cout << "Number of iterations: " << n_iter << std::endl;

    adb::run_test(adb::SumFunc(), n_iter);
    //adb::run_test(adb::SumIterFunc(), n_iter);
    //adb::run_test(adb::ProdFunc(), n_iter);
    //adb::run_test(adb::ProdIterFunc(), n_iter);
    //adb::run_test(adb::LogSumExpFunc(), n_iter);
    //adb::run_test(adb::MatrixProductFunc(), n_iter);

    return 0;
}
