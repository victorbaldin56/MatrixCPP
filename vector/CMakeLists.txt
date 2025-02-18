cmake_minimum_required(VERSION 3.14)

enable_testing()
include(GoogleTest)

add_library(vector INTERFACE)
target_include_directories(vector INTERFACE include)
target_compile_features(vector INTERFACE cxx_std_17)

add_executable(test tests/main.cc)
target_link_libraries(test GTest::gtest_main)
target_link_libraries(test vector)
gtest_discover_tests(test)
