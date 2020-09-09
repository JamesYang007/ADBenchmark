#pragma once
#include <Eigen/Dense>

namespace adb {

struct FuncBase
{
    // IMPORTANT that it is randomizing the input
    // otherwise compiler optimizes out too much in the actual benchmark
    void fill(Eigen::VectorXd& x) const 
    {
        x = Eigen::VectorXd::Random(x.size());
    }
};

} // namespace adb
