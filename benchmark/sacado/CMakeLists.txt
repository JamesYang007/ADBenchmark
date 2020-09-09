# Find Sacado13.0.0
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

# Function macro to easily create executables for each test
function(add_sacado_executable name)
    set(exec "sacado_${name}")
    add_executable(${exec} "${name}.cpp")
    target_include_directories(${exec} PRIVATE
        ${ADBENCH_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib/Trilinos/build/include)
    target_link_libraries(${exec}
        Eigen3::Eigen
        FastAD::FastAD
        ${SACADO_LIB}
        ${TEUCHOSCORE_LIB}
        ${TEUCHOSCOMM_LIB}
        benchmark::benchmark
        benchmark::benchmark_main
        pthread)
endfunction()

add_sacado_executable("log_sum_exp")
add_sacado_executable("matrix_product")
add_sacado_executable("normal_log_pdf")
add_sacado_executable("prod")
add_sacado_executable("prod_iter")
add_sacado_executable("regression")
add_sacado_executable("stochastic_volatility")
add_sacado_executable("sum")
add_sacado_executable("sum_iter")
