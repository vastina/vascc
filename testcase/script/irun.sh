#!/bin/bash

recursive() {
    local dir="$1"
    for file in "$dir"/*; do
        if [[ -d "$file" ]]; then
            # 如果是目录，则递归进入
            recursive "$file"
        elif [[ -f "$file" ]]; then
            # 如果是文件，则输出文件名
            echo "$file"
            ./vcc "$file" > test/parser/"$file.yaml"
        fi
    done
}

recursive "./testcase/invalid/"
