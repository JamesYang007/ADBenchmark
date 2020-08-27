cmake_minimum_required(VERSION 3.7)
project("ADBenchmark"
        VERSION 1.0
        DESCRIPTION "Benchmarks between various AD libraries."
        LANGUAGES C CXX)

set(ADBENCH_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/benchmark)

# Find Eigen3
find_package(Eigen3 3.3 CONFIG REQUIRED NO_MODULE)
message(STATUS "Eigen3 found at ${EIGEN3_INCLUDE_DIR}")

# Find FastAD
find_package(FastAD CONFIG REQUIRED
    HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/FastAD/build)
message(STATUS "Found FastAD config at: ${FastAD_DIR}")

# Add include/lib path to Adept2.0.8
find_path(ADEPT_INCLUDE_DIR adept)
find_library(ADEPT_LIB adept)
set(ADBENCH_INCLUDE_DIR ${ADBENCH_INCLUDE_DIR} ${ADEPT_INCLUDE_DIR})

# Automate the choosing of config
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode")

add_subdirectory(${PROJECT_SOURCE_DIR}/benchmark ${PROJECT_BINARY_DIR}/benchmark)
