#!/bin/bash

target="$1"
version="$2"
# 搜索所有以 "llvm-xxx-18" 格式命名的可执行文件
for file in /usr/bin/$target-*$version; do
    if [[ -x "$file" ]]; then
        # 获取文件名，去除路径
        filename=$(basename "$file")
        
        # 创建软连接
        sudo rm -f "/usr/bin/${filename//-$version/}"
        sudo ln -s "$file" "/usr/bin/${filename//-$version/}"
        
        echo "Created symlink for $file"
    fi
done
