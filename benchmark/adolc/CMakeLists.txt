# Find ADOL-C2.7.2
find_library(ADOLC_LIB
             adolc
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/adolc/build/lib64)
message(STATUS "Found ADOL-C lib at: ${ADOLC_LIB}")

# Function macro to easily create executables for each test
function(add_adolc_executable name)
    set(exec "adolc_${name}")
    add_executable(${exec} "${name}.cpp")
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib/adolc/build/include)
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        ${ADOLC_LIB}
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_adolc_executable("log_sum_exp")
add_adolc_executable("matrix_product")
add_adolc_executable("normal_log_pdf")
add_adolc_executable("prod")
add_adolc_executable("prod_iter")
add_adolc_executable("regression")
add_adolc_executable("stochastic_volatility")
add_adolc_executable("sum")
add_adolc_executable("sum_iter")

# ADOLC requires a .adolcrc to be copied where executables are
add_custom_target(COPYADOLCRC
    ALL
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/.adolcrc
    ${CMAKE_CURRENT_BINARY_DIR}/.adolcrc)
