#!/usr/bin/bash

clang-format -i code/*.cpp
clang-format -i code/vascc/*.cpp
clang-format -i code/include/*.hpp

#vasdef
clang-format -i code/include/base/log.hpp
clang-format -i code/include/base/String.hpp
clang-format -i code/include/base/Tree.hpp
clang-format -i code/include/base/io.hpp