add_executable(main main.cpp)
target_include_directories(main ${ADBENCH_INCLUDE_DIR})
target_link_libraries(main FastAD::FastAD Eigen3::Eigen adept)
