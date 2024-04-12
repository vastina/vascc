cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_GENERATOR "Unix Makefiles")

set(project_name vcc)
project(${project_name})

add_compile_options("-Wall" "-Wextra" "-g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories("./code/include")
file(GLOB SOURCES "./code/*.cpp")
# file(GLOB SOURCES_BASE "./code/base/*.cpp")
# add_executable(${project_name} ${SOURCES_BASE} ${SOURCES})
add_executable(${project_name} ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)