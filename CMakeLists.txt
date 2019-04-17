cmake_minimum_required(VERSION 2.6)

project(demo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(external/glm)
add_subdirectory(demo)
