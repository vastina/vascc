cmake_minimum_required(VERSION 3.9)

set(project_name cc)
project(${project_name})

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_GENERATOR "Unix Makefiles")

add_compile_options(-Wall -g)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

include_directories("./code/include")
file(GLOB SOURCES "./code/*.cpp")
file(GLOB SOURCES_BASE "./code/base/*.cpp")
add_executable(${project_name} ${SOURCES_BASE} ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)