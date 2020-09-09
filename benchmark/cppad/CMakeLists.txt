# Find CppAD20200000.3
find_library(CPPAD_LIB
             cppad_lib
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/cppad/build/lib)
message(STATUS "Found CPPAD lib at: ${CPPAD_LIB}")

# Function macro to easily create executables for each test
function(add_cppad_executable name)
    set(exec "cppad_${name}")
    add_executable(${exec} "${name}.cpp")
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib/cppad/build/include)
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        ${CPPAD_LIB}
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_cppad_executable("log_sum_exp")
add_cppad_executable("matrix_product")
add_cppad_executable("normal_log_pdf")
add_cppad_executable("prod")
add_cppad_executable("prod_iter")
add_cppad_executable("regression")
add_cppad_executable("stochastic_volatility")
add_cppad_executable("sum")
add_cppad_executable("sum_iter")
