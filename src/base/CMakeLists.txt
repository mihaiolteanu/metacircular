cmake_minimum_required(VERSION 3.5)
project(base C)
include(create_test)

add_library(base src/base.c src/stringify.c)
target_link_libraries(base input)
target_include_directories(base PUBLIC  include)
target_include_directories(base PRIVATE src)

create_test(base_test test/base_test.c base)
target_link_libraries(base_test test_helpers)
