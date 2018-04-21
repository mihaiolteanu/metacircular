cmake_minimum_required(VERSION 3.5)
project(evaluator C)
include(create_test)

add_library(evaluator src/evaluator.c)
target_link_libraries(evaluator base)
target_include_directories(evaluator PUBLIC  include)
target_include_directories(evaluator PRIVATE src)

create_test(evaluator_test test/evaluator_test.c evaluator)
target_link_libraries(evaluator_test base)

