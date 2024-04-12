-- generated by ai

set_xmakever("2.5.1")

target("vcc")
    -- 设置 C++ 标准
    set_languages("cxx20")
    -- 添加编译选项
    add_cxxflags("-Wall", "-g", "-Wextra")
    -- 添加包含目录
    add_includedirs("code/include")
    -- 添加源文件
    add_files("code/*.cpp")
    -- 设置生成目标为可执行文件
    set_kind("binary")
    -- 设置生成路径为当前目录
    set_targetdir("$(projectdir)")
    -- 设置生成目标的名称
    -- set_targetdir("./")
