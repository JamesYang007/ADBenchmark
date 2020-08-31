#include <run_test.hpp>
#include <sum.hpp>
#include <sum_iter.hpp>
#include <prod.hpp>
#include <prod_iter.hpp>
#include <log_sum_exp.hpp>
#include <matrix_product.hpp>
#include <normal_log_pdf.hpp>
#include <regression.hpp>
#include <testpack.hpp>

int main(int argc, char* argv[])
{
    using namespace adb;

    size_t n_iter = 10000;
    if (argc == 2) {
        n_iter = std::atoi(argv[1]);
    }
    std::cout << "Number of iterations: " << n_iter << std::endl;

    std::unordered_map<TestName, TestPack> packs({
        {TestName::adept,  {"adept",  false,  n_iter}},
        {TestName::adolc,  {"adolc",  false,  n_iter}},
        {TestName::cppad,  {"cppad",  false,  n_iter}},
        {TestName::sacado, {"sacado", false,  n_iter}},
        {TestName::stan,   {"stan",   true,  n_iter}},
        {TestName::fastad, {"fastad", true,  n_iter}},
    });

    // Comment out all tests except one
    // It reduces binary size and hence promotes instruction caching 

    //adb::run_test(adb::SumFunc(), packs);
    //adb::run_test(adb::SumIterFunc(), packs);
    //adb::run_test(adb::ProdFunc(), packs);
    //adb::run_test(adb::ProdIterFunc(), packs);
    //adb::run_test(adb::LogSumExpFunc(), packs);
    adb::run_test(adb::MatrixProductFunc(), packs, 64 * 1024);
    //adb::run_test(adb::NormalLogPdfFunc(), packs);
    //adb::run_test(adb::RegressionFunc(), packs);

    return 0;
}
