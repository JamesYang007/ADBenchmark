#add_compile_options(-Wno-macro-redefined -Wno-unused-local-typedef)
#add_executable(main main.cpp)
#target_include_directories(main PRIVATE ${ADBENCH_INCLUDE_DIR})
#target_link_libraries(main
#    FastAD::FastAD
#    Eigen3::Eigen
#    ${ADEPT_LIB}
#    ${ADOLC_LIB}
#    ${CPPAD_LIB}
#    ${SACADO_LIB}
#    ${TEUCHOSCORE_LIB}
#    ${TEUCHOSCOMM_LIB}
#    ${TBB_LIB}
#    ${TBBMALLOC_LIB}
#    ${TBBMALLOC_PROXY_LIB}
#    lapack
#    blas)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fastad ${CMAKE_CURRENT_BINARY_DIR}/fastad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/stan ${CMAKE_CURRENT_BINARY_DIR}/stan)
