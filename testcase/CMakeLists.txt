set(test_dir1 "./codegen")
set(test_name1 "test1")
add_executable(${test_name1} ${SOURCES} ${test_dir1}/test.cpp)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)