#!/usr/bin/bash

cmake -Bbuild .
cmake --build ./build
./testcase/script/run.sh
rm -f vcc
xmake build
./testcase/script/run.sh
./llvm/cf.sh
git add -A
# git commit
