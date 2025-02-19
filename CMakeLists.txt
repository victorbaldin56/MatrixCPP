cmake_minimum_required(VERSION 3.14)
project(Matrix)

if (${BUILD_TESTING})
  enable_testing()
endif()
find_package(GTest REQUIRED)

include(GoogleTest)

add_subdirectory(matrix)
add_subdirectory(driver)
