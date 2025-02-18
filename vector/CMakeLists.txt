cmake_minimum_required(VERSION 3.14)

enable_testing()
include(GoogleTest)

add_library(vector INTERFACE)
target_include_directories(vector INTERFACE include)
target_link_libraries(vector INTERFACE algo)
target_link_libraries(vector INTERFACE mm)
target_compile_features(vector INTERFACE cxx_std_17)

add_executable(construction tests/construction.cc)
target_link_libraries(construction GTest::gtest_main)
target_link_libraries(construction vector)
gtest_discover_tests(construction)
