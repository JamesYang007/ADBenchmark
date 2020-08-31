#pragma once
#include <string>

namespace adb {

enum class TestName
{
    adept,
    adolc,
    cppad,
    sacado,
    stan,
    fastad
};

struct TestPack
{
    std::string name;
    bool run;
    size_t n_iter;
    double time;
};

} // namespace adb
