cmake_minimum_required(VERSION 3.14)
project(Matrix)

enable_testing()
find_package(GTest REQUIRED)

include(GoogleTest)

add_subdirectory(matrix)
