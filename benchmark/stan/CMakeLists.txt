# Find STAN3.3.0 (and dependencies)
find_library(TBB_LIB
             tbb
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/tbb)
find_library(TBBMALLOC_LIB
             tbbmalloc
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/tbb)
find_library(TBBMALLOC_PROXY_LIB
             tbbmalloc_proxy
             REQUIRED
             HINTS ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/tbb)
message(STATUS "Found TBB lib at: ${TBB_LIB}")

# Function macro to easily create executables for each test
function(add_stan_executable name)
    set(exec "stan_${name}")
    add_executable(${exec} "${name}.cpp")
    target_compile_definitions(${exec} PRIVATE
        -D_REENTRANT)
    target_compile_options(${exec} PRIVATE
        -Wno-unused-local-typedef)
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib/stan-dev-math
        ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/tbb_2019_U8/include
        ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/boost_1.72.0
        ${PROJECT_SOURCE_DIR}/lib/stan-dev-math/lib/sundials_5.2.0/include)
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        ${TBB_LIB}
        ${TBBMALLOC_LIB}
        ${TBBMALLOC_PROXY_LIB}
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_stan_executable("log_sum_exp")
add_stan_executable("matrix_product")
add_stan_executable("normal_log_pdf")
add_stan_executable("prod")
add_stan_executable("prod_iter")
add_stan_executable("regression")
add_stan_executable("stochastic_volatility")
add_stan_executable("sum")
add_stan_executable("sum_iter")
