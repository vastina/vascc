//This file should not live long, it's just for testing

#include "base/vasdef.hpp"
#include "ast.hpp"
#include "lexer.hpp"

#include <memory>


using namespace vastina;

using std::unique_ptr;
using std::make_unique;


// typedef struct _cal_node{
//     token_t tk;
//     unsigned level = 0;

//     _cal_node(): tk(TOKEN::UNKNOW){};
//     _cal_node(const token_t& _tk): tk(_tk){};
// } _cal_node;

// typedef TreeNode<_cal_node> cal_node;

// enum class cal_type{
//     OPERATOR,
//     VALUE,
//     BRAC
// };
// // just temporary
// constexpr cal_type cal_token_type(TOKEN tk){
//     switch (tk){
//         case TOKEN::ADD:
//         case TOKEN::NEG:
//         case TOKEN::MULTI:
//         case TOKEN::DIV:
//         case TOKEN::LOGAND:
//         case TOKEN::LOGNOT:
//         case TOKEN::LOGOR :
//             return cal_type::OPERATOR;
//         case TOKEN::NUMBER:
//             return cal_type::VALUE;
//         case TOKEN::NLBRAC:
//         case TOKEN::NRBRAC:
//             return cal_type::BRAC;
//         default:
//             return cal_type::OPERATOR;
//     }
// }


// inline cal_node::pointer parser_cal(const unique_ptr<std::vector<vastina::token_t>>& tks, unsigned& offset, unsigned size = 0){
    
//     static BracketCount bc;
//     static unsigned MaxSize = size ? size:tks->size();

//     auto root = new cal_node(tks->at(offset));
//     root->data.level = Level(root->data.tk.token);
//     if(offset+1 >= MaxSize) return root;
    
//     while(true){
//         auto current = new cal_node(tks->at(offset));
//         //cal_type type = cal_token_type(root->data.tk.token);
//         offset++;
//         switch (cal_token_type(current->data.tk.token)){
//             case cal_type::BRAC:{
//                 if(current->data.tk.token == TOKEN::NRBRAC){    
//                     if(++bc.close > bc.open) 
//                         return nullptr;
//                     root->data.level = 0;
//                     return root;
//                 }else {
//                     ++bc.open;
//                     if(cal_type::BRAC == cal_token_type(root->data.tk.token)) root = parser_cal(tks, offset);
//                     else current = parser_cal(tks, offset);
//                 }
//                 break;
//             }
//             case cal_type::VALUE:{
//                 if(root->data.tk.token == TOKEN::NUMBER) break;

//                 auto temp = root->FindChildR(
//                     [](const cal_node::pointer _node) {return (_node->right == nullptr);}
//                 );

//                 if(cal_type::OPERATOR != cal_token_type(temp->data.tk.token)) return nullptr;
                
//                 temp->InsertRight(current);

//                 break;
//             }
//             case cal_type::OPERATOR:{
//                 current->data.level = Level(current->data.tk.token); 
//                 if(current->data.level >= root->data.level){
//                     root->ReplaceByL(current);
//                     root = current;
//                 }
//                 else{
//                     auto temp = root->FindChildR(
//                         [&current](const cal_node::pointer _node)->bool {
//                             return (_node->right == nullptr)||(_node->data.level <= current->data.level);
//                     });
//                     if(temp != root)//current取代原node，原node成为current的left
//                         temp->ReplaceByL(current);
//                     else
//                         temp->InsertRight(current);
//                 }

//                 break;
//             }

//             default:
//                 return nullptr;
//         }
        
//         if(offset >= MaxSize) break;

//     }

//     return root;
// }

// inline int cal(const cal_node::pointer root){
//     switch (root->data.tk.token)
//     {
//     case TOKEN::ADD:
//         return cal(root->left) + cal(root->right);
//         break;
//     case TOKEN::NEG:
//         return cal(root->left) - cal(root->right);
//         break;
//     case TOKEN::MULTI:
//         return cal(root->left) * cal(root->right);
//         break;
//     case TOKEN::DIV:
//         return cal(root->left) / cal(root->right);
//         break;
//     case TOKEN::NUMBER:
//         return std::stoi(root->data.tk.data);
//         break;
//     case TOKEN::AND:
//         return cal(root->left) & cal(root->right);
//         break;
//     case TOKEN::OR:
//         return cal(root->left) | cal(root->right);
//         break;
//     default:
//         return 0;
//         break;
//     }
// }

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
            int val = temp.getValue();
            tempnode->InsertLeft(new assign_node(_assign_node(TOKEN::NUMBER, val)));
            update_val(root, val);
            
            break;
        }
    }

    return root;
}