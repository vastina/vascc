cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_GENERATOR "Unix Makefiles")

set(project_name vcc)
project(${project_name})

add_compile_options(
    "-Wall" "-Wextra" "-g" 
    "-lfolly"  "-lglog" "-lfmt"
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(project_dir "./code")
include_directories("${project_dir}/include")
file(GLOB SOURCES "${project_dir}/vascc/*.cpp")

add_executable(${project_name} ${SOURCES} ${project_dir}/main.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/)

add_subdirectory(${CMAKE_SOURCE_DIR}/testcase)