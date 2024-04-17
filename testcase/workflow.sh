#!/usr/bin/bash

cmake -Bbuild .
cmake --build ./build
./testcase/run.sh
rm -f vcc
# xmake build
# ./testcase/run.sh
./llvm/cf.sh
git add -A
git commit
