cmake_minimum_required(VERSION 3.14)

add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE include)
target_compile_features(matrix INTERFACE cxx_std_20)
target_link_libraries(matrix INTERFACE vector)
