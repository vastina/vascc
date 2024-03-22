#include "lexer.hpp"

#include <memory>

using namespace vastina;
int main(){
    lexer lx = lexer("./stage_1/valid/newlines.c");

    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();
    lx.Next();

    auto tks = std::make_unique<std::vector<token_t>>(lx.getTokens());
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<'\n';
    }
}