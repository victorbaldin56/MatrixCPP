cmake_minimum_required(VERSION 3.14)
project(Matrix)

add_subdirectory(vector)
add_subdirectory(matrix)
add_subdirectory(driver)

option(ENABLE_TESTS "Enable testing" ON)
if (ENABLE_TESTS)
  find_package(GTest REQUIRED)

  include(GoogleTest)
  enable_testing()
  add_subdirectory(tests)
endif()
