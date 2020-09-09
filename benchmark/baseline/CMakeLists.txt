# Function macro to easily create executables for each test
function(add_baseline_executable name)
    set(exec "baseline_${name}")
    add_executable(${exec} "${name}.cpp")
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR})
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_baseline_executable("log_sum_exp")
add_baseline_executable("matrix_product")
add_baseline_executable("normal_log_pdf")
add_baseline_executable("prod")
add_baseline_executable("prod_iter")
add_baseline_executable("regression")
add_baseline_executable("stochastic_volatility")
add_baseline_executable("sum")
add_baseline_executable("sum_iter")
