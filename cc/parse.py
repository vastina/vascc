# auth：ykiko
# link：https://zhuanlan.zhihu.com/p/669360731

import clang.cindex as CX
import sys

def traverse(node: CX.Cursor, prefix="", is_last=True):
    branch = "└──" if is_last else "├──"
    text = f"{str(node.kind).removeprefix('CursorKind.')}: {node.spelling}"

    if node.kind == CX.CursorKind.INTEGER_LITERAL:
        value = list(node.get_tokens())[0].spelling
        text = f"{text}{value}"

    print(f"{prefix}{branch} {text}")
    new_prefix = prefix + ("    " if is_last else "│   ")
    children = list(node.get_children())

    # 遍历子节点
    for child in children:
        traverse(child, new_prefix, child is children[-1])


index = CX.Index.create(excludeDecls=True)

if __name__ == "__main__":
    # 获取命令行参数列表
    args = sys.argv
    tu = index.parse(args[1], args=['-std=c99'])
    traverse(tu.cursor)

