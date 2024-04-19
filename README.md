# Write a Toy Compiler!

## 28/3/24
```
git reset --hard HEAD^
git add -A
git commit -m "..."
git push --force
```
## 31/3/24
Goodbye March, I thought I could finish the compiler in a week, for a very mini version

## 4/9/24
expression parse could be a very small part

## 4/10/24
func keyword helps to avoid SYMBOLF check in lexer if taking both functions and variables as symbol.But I still want to parse function declare that starts with type in the future.C

## 4/11/24
symbol.cpp is going to be shit if no rebuild in the future

## 4/13/24
using binary expression is better than using calculate and assign

## 4/19/24
```bash
git log --oneline
# find my first commit
git checkout --orphan new-branch-name commit-hash
git reset
git add .
git commit -m "New initial commit based on ${commit-hash}"
git log ${commit-hash}..${old-branch-name} --oneline
# 重新应用那次提交之后的commit
git cherry-pick xxxxxxx
```