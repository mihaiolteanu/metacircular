cmake_minimum_required(VERSION 3.5)
project(input C)
include(create_test)

add_library(input src/input.c)
target_link_libraries(input base)
target_include_directories(input PUBLIC  include)
target_include_directories(input PRIVATE src)

create_test(input_test test/input_test.c input)
