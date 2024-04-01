#!/bin/bash

# 搜索所有以 "llvm-xxx-18" 格式命名的可执行文件
for file in /usr/bin/llvm-*18; do
    if [[ -x "$file" ]]; then
        # 获取文件名，去除路径
        filename=$(basename "$file")
        
        # 创建软连接
        sudo ln -s "$file" "/usr/bin/${filename//-18/}"
        
        echo "Created symlink for $file"
    fi
done
