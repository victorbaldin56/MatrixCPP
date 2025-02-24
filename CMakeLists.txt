cmake_minimum_required(VERSION 3.14)
project(Matrix)

if (${BUILD_TESTING})
  enable_testing()
  find_package(GTest REQUIRED)
  find_package(Boost COMPONENTS process REQUIRED)
  include(GoogleTest)
endif()

add_subdirectory(vector)
add_subdirectory(matrix)
add_subdirectory(driver)

if (${BUILD_TESTING})
  add_subdirectory(tests)
endif()
