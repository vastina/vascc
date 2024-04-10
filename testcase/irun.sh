#!/usr/bin/bash

for file in `ls ./testcase/invalid/`; do
    echo  reading $file... 
    ./cc ./testcase/valid/$file
done