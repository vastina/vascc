#!/usr/bin/bash

rm -f vcc
cmake -Bbuild .
cmake --build ./build
./testcase/script/run.sh
rm -f vcc
xmake build
./testcase/script/run.sh
bash ~/bash/cf.sh ./code

# git add -A
# git commit
