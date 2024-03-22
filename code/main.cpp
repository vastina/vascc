#include "lexer.hpp"

#include <memory>

using namespace vastina;
int main(){
    lexer lx = lexer("./stage_1/invalid/no_brace.c");

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

    const std::vector<token_t> *tks = lx.getTokens();
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<'\n';
    }
}