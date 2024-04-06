//This file should not live long, it's just for testing

#include "base/vasdef.hpp"
#include "expr.hpp"
#include "lexer.hpp"

#include <memory>


using namespace vastina;

using std::unique_ptr;
using std::make_unique;


enum class assign_type{
    OPERATOR,
    EXPRSSION,
    VARIABLE
};//primary tokens should be processed

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

inline void update_val(assign_node::pointer root, int val){
    if(root == nullptr) return;
        root->data.val = val;
    update_val(root->left, val);
    update_val(root->right, val);
}

inline assign_node::pointer parser_assign(const unique_ptr<std::vector<vastina::token_t>>& tks, unsigned& offset, unsigned size = 0){

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
            auto temp = CalExpression<int>(ExpressionUnit(tks.get(), offset, maxsize));
            temp.Parse();   temp.Calculate();   temp.Walk(walk_order::PREORDER);

            auto tempnode = root->FindChildL([](const assign_node::pointer _node){
                return (_node->left == nullptr);
            });
            int val = temp.getValue_copy();
            tempnode->InsertLeft(new assign_node(_assign_node(TOKEN::NUMBER, val)));
            update_val(root, val);
            
            break;
        }
    }

    return root;
}