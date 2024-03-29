#include "main.hpp"
#include "ast.hpp"
#include "base/Tree.hpp"
#include "base/vasdef.hpp"

#include <iostream>

//primary tokens need to be processed


const static before_main_t before_main = before_main_t();

enum class if_type{
    _0
};

typedef struct _if_node{

} _if_node;

typedef TreeNode<_if_node> if_node;

constexpr if_type if_token_type(TOKEN tk){
    switch (tk){
        default:
            return if_type::_0;
            break;
    }
}

if_node::pointer parser_if(std::unique_ptr<std::vector<token_t>>& tks, unsigned& offset){
    if_node::pointer root = nullptr;
    return root;
}


int main(int argc, char* argv[]){
    // if(argc != 2){
    //     std::cout << "Usage: " << argv[0] << " <filename>\n";
    //     return 1;
    // }
    // lexer lx = lexer(argv[1]);

    lexer lx = lexer("./testcase/test.txt");
    // ans =abcdefg = ((2+8)|5 + 3*7/ 4 -9 & 6) -5;

    while (lexer::STATE::END != lx.Next()) ;

    auto tks = make_unique<std::vector<token_t>>(lx.getTokens());
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<' '<< tks->at(i).line << '\n';
    }

    unsigned offset = 0;
    auto root = parser_assign(tks, offset);
    if(root == nullptr){
        std::cout << "Error\n";
        return 1;
    }
    

    std::cout << '\n';
    root->Walk(walk_order::PREORDER, [](const _assign_node& data_){
        std::cout  << data_.tk.data << ' ' << data_.val << '\n';
    });


    // CalExpression<int> test = CalExpression<int>( ExpressionUnit(std::make_shared<std::vector<token_t>>(lx.getTokens()), 0, 0) );
    // auto a = test.getValue();
    // std::cout << a <<'\n';

    return 0;
    
}