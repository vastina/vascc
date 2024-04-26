#!/usr/bin/bash

clang-format -i code/*.cpp
clang-format -i code/vascc/*.cpp
clang-format -i code/include/*.hpp
clang-format -i testcase/codegen/*.cpp
clang-format -i code/include/base/*