#include "main.hpp"
#include "ast.hpp"
#include "base/vasdef.hpp"

#include <iostream>

const static before_main_t before_main = before_main_t();

enum class assign_type{
    OPERATOR,
    EXPRSSION,
    VARIABLE
};//primary tokens are processed

typedef struct _assign_node{
    int val; //this is really bad, I need to learn template programming
    token_t tk;
//    unsigned level = 0;

    _assign_node(): tk(TOKEN::UNKNOW){};
    _assign_node(const token_t& _tk): val(0), tk(_tk){};
    _assign_node(TOKEN _tk, int _val): val(_val), tk(_tk){};
} _assign_node;

typedef TreeNode<_assign_node> assign_node;


constexpr assign_type assign_token_type(TOKEN tk){
    switch (tk){
        case TOKEN::ASSIGN:
            return assign_type::OPERATOR;
        case TOKEN::SYMBOL:
            return assign_type::VARIABLE;
        default:
            return assign_type::EXPRSSION;
    }
}

void update_val(assign_node::pointer root, int val){
    if(root == nullptr) return;
        root->data.val = val;
    update_val(root->left, val);
    update_val(root->right, val);
}

assign_node::pointer parser_assign(const unique_ptr<std::vector<vastina::token_t>>& tks, unsigned& offset, unsigned size = 0){

    static unsigned MaxSize = size ? size:tks->size();
    //static unsigned Base = offset;

    auto root = new assign_node(tks->at(offset));
    if(offset+1 >= MaxSize) return root;

    while(true){
        auto current = new assign_node(tks->at(offset));
        if(assign_token_type(current->data.tk.token) == assign_type::VARIABLE){
            if(tks->at(offset+1).token == TOKEN::ASSIGN){
                auto temp = new assign_node(tks->at(offset+1));
                if(assign_type::OPERATOR == assign_token_type(root->data.tk.token))
                    temp->InsertLeft(root);
                temp->InsertRight(current);
                root = temp;
                offset += 2;
            }
            else return nullptr;
        }
        else if(assign_token_type(current->data.tk.token) == assign_type::EXPRSSION){
            unsigned maxsize = offset;
            while(tks->at(++maxsize).token != TOKEN::SEMICOLON)
                if(maxsize+1 >= MaxSize) return nullptr;
            auto temp = parser_cal(tks, offset, maxsize);
            if(temp == nullptr) return nullptr;

            auto tempnode = root->FindChildL([](const assign_node::pointer _node){
                return (_node->left == nullptr);
            });
            int val = cal(temp);
            tempnode->InsertLeft(new assign_node(_assign_node(TOKEN::NUMBER, val)));
            update_val(root, val);
            
            break;
        }
    }

    return root;
}

int main(int argc, char* argv[]){
    // if(argc != 2){
    //     std::cout << "Usage: " << argv[0] << " <filename>\n";
    //     return 1;
    // }
    // lexer lx = lexer(argv[1]);

    lexer lx = lexer("./test/test");
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

    return 0;
}