cmake_minimum_required(VERSION 3.5)
project(metacircular_evaluator C)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/common)

include(CTest)
add_subdirectory(src)
add_subdirectory(vendor)

