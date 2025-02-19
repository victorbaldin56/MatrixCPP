cmake_minimum_required(VERSION 3.14)

add_executable(driver main.cc)
target_link_libraries(driver matrix)
