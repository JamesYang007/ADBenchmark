#include <run_test.hpp>
#include <sum.hpp>

int main(int argc, char* argv[])
{
    size_t n_iter = 10000;
    if (argc == 2) {
        n_iter = std::atoi(argv[1]);
    }
    std::cout << "Number of iterations: " << n_iter << std::endl;
    adb::run_test(adb::SumFunc(), n_iter);
    return 0;
}
