cmake_minimum_required(VERSION 3.5)
project(environment C)
include(create_test)

add_library(environment src/environment.c)
target_link_libraries(environment object)
target_include_directories(environment PUBLIC  include)
target_include_directories(environment PRIVATE src)

create_test(environment_test test/environment_test.c environment)
