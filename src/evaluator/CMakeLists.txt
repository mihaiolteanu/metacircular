cmake_minimum_required(VERSION 3.5)
project(evaluator C)
include(create_test)

add_library(evaluator src/evaluator.c)
target_link_libraries(evaluator object)
target_link_libraries(evaluator environment)
target_include_directories(object PUBLIC  include)
target_include_directories(object PRIVATE src)

create_test(evaluator_test test/evaluator_test.c evaluator)
target_link_libraries(evaluator_test environment)
target_link_libraries(evaluator_test object)
