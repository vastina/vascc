#!/usr/bin/bash

for file in `ls ./testcase/valid/`; do
    echo  reading $file... 
    ./cc ./testcase/valid/$file
done