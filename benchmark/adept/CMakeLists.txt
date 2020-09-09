# Find Adept2.0.8
find_library(ADEPT_LIB
             NAMES libadept.a
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/adept-2.0.8/build/lib)
message(STATUS "Found Adept lib at: ${ADEPT_LIB}")

# Function macro to easily create executables for each test
function(add_adept_executable name)
    set(exec "adept_${name}")
    add_executable(${exec} "${name}.cpp")
    target_compile_definitions(${exec} PRIVATE
        -DADEPT_STACK_THREAD_UNSAFE)
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib/adept-2.0.8/build/include)
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        ${ADEPT_LIB}
        lapack
        blas
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_adept_executable("log_sum_exp")
add_adept_executable("matrix_product")
add_adept_executable("normal_log_pdf")
add_adept_executable("prod")
add_adept_executable("prod_iter")
add_adept_executable("regression")
add_adept_executable("stochastic_volatility")
add_adept_executable("sum")
add_adept_executable("sum_iter")
