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
    -DEIGEN_NO_DEBUG
    # required by adept
    -DADEPT_STACK_THREAD_UNSAFE)

if (STOCHASTIC_VOLATILITY_BYTESIZE)
    add_definitions(-DSTOCHASTIC_VOLATILITY_BYTESIZE)
endif()

# Find GBench
find_package(benchmark CONFIG REQUIRED
    HINTS ${PROJECT_SOURCE_DIR}/lib/benchmark/build/lib/cmake)

# Find Eigen3
find_package(Eigen3 3.3 CONFIG REQUIRED NO_MODULE)
message(STATUS "Found Eigen3 config at: ${EIGEN3_INCLUDE_DIR}")

# Find Adept2.0.8
set(ADBENCH_INCLUDE_DIR
    ${ADBENCH_INCLUDE_DIR} ${ADEPT_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/adept-2.0.8/build/include)
find_library(ADEPT_LIB
             NAMES libadept.a
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/adept-2.0.8/build/lib)
message(STATUS "Found Adept lib at: ${ADEPT_LIB}")

# Find ADOL-C2.7.2
set(ADBENCH_INCLUDE_DIR
    ${ADBENCH_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/adolc/build/include)
find_library(ADOLC_LIB
             adolc
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/adolc/build/lib64)
message(STATUS "Found ADOL-C lib at: ${ADOLC_LIB}")

# Find CppAD20200000.3
set(ADBENCH_INCLUDE_DIR
    ${ADBENCH_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/cppad/build/include)
find_library(CPPAD_LIB
             cppad_lib
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/cppad/build/lib)
message(STATUS "Found CPPAD lib at: ${CPPAD_LIB}")

# Find Sacado13.0.0
set(ADBENCH_INCLUDE_DIR
    ${ADBENCH_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/Trilinos/build/include)
find_library(SACADO_LIB
             sacado
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/Trilinos/build/lib)
find_library(TEUCHOSCORE_LIB
             teuchoscore
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/Trilinos/build/lib)
find_library(TEUCHOSCOMM_LIB
             teuchoscomm
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/Trilinos/build/lib)
message(STATUS "Found Sacado lib at: ${SACADO_LIB}")
message(STATUS "Found Teuchoscore lib at: ${TEUCHOSCORE_LIB}")
message(STATUS "Found Teuchoscomm lib at: ${TEUCHOSCOMM_LIB}")

# Automate the choosing of config
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode")

add_subdirectory(${PROJECT_SOURCE_DIR}/benchmark ${PROJECT_BINARY_DIR}/benchmark)
