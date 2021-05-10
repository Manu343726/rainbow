cmake_minimum_required(VERSION 3.15)
project(rainbow)

include(cmake/config.cmake)
include(cmake/externals.cmake)

add_subdirectory(src)
add_subdirectory(examples)
