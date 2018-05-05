# Common test functions. Easier to use, less verbiage.
cmake_minimum_required(VERSION 3.5)
project(test_helpers C)

add_library(test_helpers src/test_helpers.c)
target_link_libraries(test_helpers unity)
target_link_libraries(test_helpers base)
target_include_directories(test_helpers PUBLIC  include)
target_include_directories(test_helpers PRIVATE src)
