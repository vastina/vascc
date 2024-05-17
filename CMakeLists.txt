cmake_minimum_required(VERSION 3.9)

set(CMAKE_C_COMPILER "/usr/bin/gcc-14")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-14")
set(CMAKE_GENERATOR "Unix Makefiles")

set(project_name vcc)
project(${project_name})

add_compile_options(
    "-Wall" "-Wextra"
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(project_dir "./code")
include_directories("${project_dir}/include")

file(GLOB SOURCES_BASE "${project_dir}/vascc/base/*.cpp")
file(GLOB SOURCES "${project_dir}/vascc/*.cpp")
add_library(code_objects OBJECT ${SOURCES} ${SOURCES_BASE})

add_executable(${project_name} ${project_dir}/main.cpp)
target_link_libraries(${project_name} code_objects)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)

add_subdirectory(${CMAKE_SOURCE_DIR}/testcase)