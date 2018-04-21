cmake_minimum_required(VERSION 3.5)
project(base C)
include(create_test)

add_library(base src/base.c)
target_include_directories(base PUBLIC  include)
target_include_directories(base PRIVATE src)

create_test(base_test test/base_test.c base)
