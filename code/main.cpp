#include "lexer.hpp"

#include <iostream>
#include <memory>

using namespace vastina;
int main(int argc, char* argv[]){
    // if(argc != 2){
    //     std::cout << "Usage: " << argv[0] << " <filename>\n";
    //     return 1;
    // }
    lexer lx = lexer("./stage_7/valid/nested_scope.c");

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
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<' '<< tks->at(i).line << '\n';
    }
}