set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

set(test_dir1 "./codegen")

set(hello "hello")
add_executable(${hello} ${test_dir1}/hello.cpp)
target_link_libraries(${hello} code_objects)
set(binary "binary")
add_executable(${binary} ${test_dir1}/binary.cpp)
target_link_libraries(${binary} code_objects)