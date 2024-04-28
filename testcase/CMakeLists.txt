set(test_dir1 "./codegen")

set(test1 "hello")
add_executable(${test1} ${test_dir1}/hello.cpp)
target_link_libraries(${test1} code_objects)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)