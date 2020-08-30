add_compile_options(-Wno-macro-redefined)
add_executable(main main.cpp)

if (AD_ASSEMBLY)
    message(STATUS "GENERATING ASSEMBLY")
    target_compile_options(main PRIVATE -save-temps)
endif()

target_include_directories(main PRIVATE ${ADBENCH_INCLUDE_DIR})
target_link_libraries(main
    FastAD::FastAD
    Eigen3::Eigen
    ${ADEPT_LIB}
    ${ADOLC_LIB}
    ${CPPAD_LIB}
    ${SACADO_LIB}
    ${TEUCHOSCORE_LIB}
    ${TEUCHOSCOMM_LIB}
    ${TBB_LIB}
    ${TBBMALLOC_LIB}
    ${TBBMALLOC_PROXY_LIB}
    lapack
    blas)
