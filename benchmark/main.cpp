#ifndef NITER
#define NITER 10000
#endif

#include <run_test.hpp>
#include <sum.hpp>

int main()
{
    adb::run_test(adb::SumFunc(), NITER);
    return 0;
}
