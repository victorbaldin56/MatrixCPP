cmake_minimum_required(VERSION 3.14)
project(Matrix)

if (${BUILD_TESTING})
  enable_testing()
endif()
find_package(GTest REQUIRED)

include(GoogleTest)

function(TEST_LIBRARY NAME)
  file(GLOB TEST_SRCS tests/*.cc)
  foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} ${NAME} GTest::gtest_main)
    gtest_discover_tests(${TEST_NAME})
  endforeach()
endfunction()

add_subdirectory(vector)
add_subdirectory(matrix)
add_subdirectory(driver)

add_test(NAME e2e
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/run_e2e_test.py)
