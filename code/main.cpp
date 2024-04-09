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
std::cout <<"--------------------------lexer--------------------------------\n";
    lexer lx = lexer(argv[1]);
    while (lexer::STATE::END != lx.Next()) ;

    auto tks = std::make_unique<std::vector<token_t>>(lx.getTokens());
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<' '<< tks->at(i).line << '\n';
    }
std::cout <<"--------------------------lexer--------------------------------\n";

std::cout <<"--------------------------preprocess--------------------------------\n";
    Preprocess *pp = new Preprocess(*tks);
    pp->Process();

    for(unsigned i=0; i<pp->getSize(); i++){
        auto& next = pp->getNext();
        std::cout << Preprocess::p_token_str(next.tk) <<' ';
        for(unsigned j=next.start; j<next.end; j++){
            std::cout << tks->at(j).data << ' ';
        }
        std::cout << '\n';
    }
std::cout <<"--------------------------preprocess--------------------------------\n";

    std::queue<decltype(pp->CurrentScope())> st;
    st.push(pp->CurrentScope());

    while (!st.empty())
    {
        auto scope = st.front();  st.pop();
        std::cout << scope->getRange().start <<' '<< scope->getRange().end << '\n';
        for(auto&& child: scope->getChildren()) st.push(child);
    }
    
    // unsigned offset = 0;
    // auto root = parser_assign(tks, offset);
    // if(root == nullptr){
    //     std::cout << "Error\n";
    //     return 1;
    // }
    

    // std::cout << '\n';
    // root->Walk(walk_order::PREORDER, [](const _assign_node& data_){
    //     std::cout  << data_.tk.data << ' ' << data_.val << '\n';
    // });

    return 0;
    
}