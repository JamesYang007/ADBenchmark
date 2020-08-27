#pragma once
#include <string>
#include <Eigen/Dense>
#include <fastad>

namespace adb {

struct SumFunc
{
    template <class T>
    T operator()(const Eigen::Matrix<T, Eigen::Dynamic, 1>& x) const
    {
        T sum_x = 0;
        for (int i = 0; i < x.size(); ++i) {
            sum_x += x(i);
        }
        return sum_x;
    }

    template <class T, class S>
    auto operator()(const ad::VarView<T, S>& x) const
    {
        return ad::sum(x);
    }

    std::string name() const { return "sum"; }

    void fill(Eigen::VectorXd& x) {
        for (int i = 0; i < x.size(); ++i) {
            x(i) = i;
        }
    }
};

} // namespace adb
