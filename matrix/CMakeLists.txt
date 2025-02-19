cmake_minimum_required(VERSION 3.14)

add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE include)
target_compile_features(matrix INTERFACE cxx_std_11)

file(GLOB TEST_SRCS tests/*.cc)
foreach(TEST_SRC ${TEST_SRCS})
  get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SRC})
  target_link_libraries(${TEST_NAME} matrix GTest::gtest_main)
  gtest_discover_tests(${TEST_NAME})
endforeach()
