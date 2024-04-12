#!/usr/bin/bash

for file in `ls ./testcase/valid/`; do
    echo  reading $file... 
    ./vcc ./testcase/valid/$file
done