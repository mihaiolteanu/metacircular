cmake_minimum_required(VERSION 3.5)
project(object C)
include(create_test)

add_library(object src/object.c)
target_include_directories(object PUBLIC  include)
target_include_directories(object PRIVATE src)

create_test(object_test test/object_test.c object)
