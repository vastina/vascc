## Todo List

### NOT FINISHED：
- [ ] make lexer more scalable(sth like read .in file, add keyword after compile)
- [ ] I need to get rid of offset, it's so stupid
- [ ] type class instead of TOKEN as the result of getType()
- [ ] walk throught the symbol table to know how many space is needed and `subq ${}, %rsp` at start of func
- [ ] go back and finish more features

### FINISHED：
- [ ] process primary Tokens into checked and ranged tokens
- [ ] finish the Scope class which need full use of the range got from processed primary tokens
- [ ] a hellocodegen.cpp to generate the code for basic printf
