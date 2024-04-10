#include "lexer.hpp"
#include "expr.hpp"
#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <iostream>
#include <queue>
#include <memory>

//const static before_main_t before_main = before_main_t();


using namespace vastina;
int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    lexer lx = lexer(argv[1]);
std::cout <<"--------------------------lexer-------------------------------------\n";
    while (lexer::STATE::END != lx.Next()) ;

    auto tks = std::make_unique<std::vector<token_t>>(lx.getTokens());
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<' '<< tks->at(i).line << '\n';
    }
std::cout <<"--------------------------preprocess--------------------------------\n";
    Preprocess *pp = new Preprocess(*tks, lx.getScope());
    pp->Process();

    for(unsigned i=0; i<pp->getSize(); i++){
        auto& next = pp->getNext();
        std::cout <<i <<' '<< Preprocess::p_token_str(next.tk) <<' ';
        for(unsigned j=next.start; j<next.end; j++){
            std::cout << tks->at(j).data << ' ';
        }
        std::cout << '\n';
    }
std::cout <<"--------------------------preprocess-result-------------------------\n";

    std::queue<decltype(pp->CurrentScope())> st;
    st.push(pp->CurrentScope());

    while (!st.empty())
    {
        auto scope = st.front();  st.pop();
        auto table = scope->getSymbolTable();
        std::cout << scope->getRange().start <<' '<< scope->getRange().end << '\n';
        std::cout << "vars: ";
        for(auto&& var:table.Variables) std::cout << var.first <<' '; 
            std::cout <<'\n';
        std::cout << "funs: ";
        for(auto&&  fc:table.functions) std::cout << fc.first  <<' '; 
            std::cout <<'\n';
        for(auto&& child: scope->getChildren()) st.push(child);
    }

std::cout <<"--------------------------------------------------------------------\n";



    // std::string filename{argv[1]};
    // filename += ".s";

    return 0;
}