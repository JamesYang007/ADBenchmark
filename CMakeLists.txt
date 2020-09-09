cmake_minimum_required(VERSION 3.7)
project("ADBenchmark"
        VERSION 1.0
        DESCRIPTION "Benchmarks between various AD libraries."
        LANGUAGES C CXX)

set(ADBENCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/benchmark)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -march=native)

add_definitions(
    -DNDEBUG
    -DEIGEN_NO_DEBUG)

# TODO: move this?
if (STOCHASTIC_VOLATILITY_BYTESIZE)
    add_definitions(-DSTOCHASTIC_VOLATILITY_BYTESIZE)
endif()

# Find GBench
find_package(benchmark CONFIG REQUIRED
    HINTS ${PROJECT_SOURCE_DIR}/lib/benchmark/build/lib/cmake)

# Find Eigen3
find_package(Eigen3 3.3 CONFIG REQUIRED NO_MODULE)
message(STATUS "Found Eigen3 config at: ${EIGEN3_INCLUDE_DIR}")

# Find FastAD
find_package(FastAD CONFIG REQUIRED
    HINTS ${PROJECT_SOURCE_DIR}/lib/FastAD/build)
message(STATUS "Found FastAD config at: ${FastAD_DIR}")

# Automate the choosing of config
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode")

add_subdirectory(${PROJECT_SOURCE_DIR}/benchmark ${PROJECT_BINARY_DIR}/benchmark)
