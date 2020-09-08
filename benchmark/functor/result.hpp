#pragma once
#include <string>
#include <ostream>
#include <unordered_map>
#include <testpack.hpp>

namespace adb {

inline void print_results_header(
        std::ostream& fs,
        const std::unordered_map<adb::TestName, adb::TestPack>& packs)
{
    fs << 'N';
    for (auto& pair : packs) {
        if (pair.second.run) {
            fs << ',' << pair.second.name;
        }
    }
    fs << ",double\n";
}

inline void print_results(
        std::ostream& fs,
        const std::unordered_map<adb::TestName, adb::TestPack>& packs)
{
    for (auto& pair : packs) {
        if (pair.second.run) {
            fs << ',' << pair.second.time;
        }
    }
}

} // namespace adb
