#include <Eigen/Dense>
#include <iostream>

namespace adb {

inline void check_gradient(const Eigen::VectorXd& actual,
                           const Eigen::VectorXd& expected,
                           const std::string& name)
{
    auto diff = (actual.array() - expected.array()).abs();
    if ((actual.array() != expected.array()).any()) {
        std::cerr << "WARNING (" << name << ") MAX ABS ERROR PROP: " 
                  << (diff / expected.array()).maxCoeff() << std::endl;
    }
}

} // namespace adb
