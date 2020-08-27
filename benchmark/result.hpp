#pragma once
#include <string>
#include <ostream>

namespace adb {

void print_results_header(std::ostream& fs)
{
    fs << "N"
          ",adept"
          ",fastad"
          "\n";
}

} // namespace adb
