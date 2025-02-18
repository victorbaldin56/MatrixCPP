cmake_minimum_required(VERSION 3.14)

add_library(mm INTERFACE)
target_include_directories(mm INTERFACE include)
target_compile_features(mm INTERFACE cxx_std_11)
