cmake_minimum_required(VERSION 3.14)

add_library(vector INTERFACE)
target_include_directories(vector INTERFACE include)
target_compile_features(vector INTERFACE cxx_std_17)

test_library(vector)
