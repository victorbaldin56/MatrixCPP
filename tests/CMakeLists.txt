cmake_minimum_required(VERSION 3.14)

add_executable(vector_unit_test src/vector_unit_test.cc)
target_link_libraries(vector_unit_test vector GTest::gtest_main)

add_executable(matrix_unit_test src/matrix_unit_test.cc)
target_link_libraries(matrix_unit_test matrix GTest::gtest_main)

gtest_discover_tests(vector_unit_test)
gtest_discover_tests(matrix_unit_test)

# end-to-end
add_test(NAME e2e
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/run_e2e_test.py)
