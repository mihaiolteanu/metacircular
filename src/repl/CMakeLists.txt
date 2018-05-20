cmake_minimum_required(VERSION 3.5)
project(repl C)
include(create_test)

add_executable(repl src/repl.c)
target_link_libraries(repl input)
target_link_libraries(repl evaluator)
target_link_libraries(repl readline)
target_include_directories(repl PUBLIC include)
target_include_directories(repl PRIVATE src)

# create_test(repl_test test/repl_test.c input)
# target_link_libraries(repl_test test_helpers)
