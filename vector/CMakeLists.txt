cmake_minimum_required(VERSION 3.14)

if (${BUILD_TESTING})
  enable_testing()
endif()

include(GoogleTest)

add_library(vector INTERFACE)
target_include_directories(vector INTERFACE include)

add_executable(test tests/main.cc)
target_link_libraries(test GTest::gtest_main)
target_link_libraries(test vector)
gtest_discover_tests(test)
