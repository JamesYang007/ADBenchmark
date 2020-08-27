#pragma once
#include <string>
#include <ostream>

namespace adb {

void print_results_header(std::ostream& fs)
{
    fs << "N"
          ",adept"
          ",adolc"
          ",cppad"
          ",sacado"
          ",stan"
          ",fastad"
          ",double"
          "\n";
}

} // namespace adb
