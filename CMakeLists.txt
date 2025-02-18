cmake_minimum_required(VERSION 3.14)
project(Matrix)

find_package(GTest REQUIRED)

add_subdirectory(memory_manager)
add_subdirectory(algo)
add_subdirectory(vector)
add_subdirectory(matrix)
