cmake_minimum_required(VERSION 3.14)
project(Matrix)

if (${BUILD_TESTING})
  enable_testing()
endif()
find_package(GTest REQUIRED)

include(GoogleTest)

add_subdirectory(matrix)
add_subdirectory(driver)

add_test(NAME e2e
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/run_e2e_test.py)
