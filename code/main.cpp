#include "base/vasdef.hpp"
#include "base/Tree.hpp"
#include "lexer.hpp"

#include <iostream>
#include <memory>

using namespace vastina;

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

typedef struct _node{
    token_t tk;
    unsigned level = 0;

    _node(): tk(TOKEN::UNKNOW){};
    _node(token_t _tk): tk(_tk){};
} _node;

typedef TreeNode<_node> node;

typedef struct BracketCount{
    unsigned open = 0;
    unsigned close = 0;
} BracketCount;

enum class _M_type{
    OPERATOR,
    NUM,
    BRAC
};
// just temporary
constexpr _M_type _token_type(TOKEN tk){
    switch (tk){
        case TOKEN::ADD:
        case TOKEN::NEG:
        case TOKEN::MULTI:
        case TOKEN::DIV:
        case TOKEN::LOGAND:
        case TOKEN::LOGNOT:
        case TOKEN::LOGOR :
            return _M_type::OPERATOR;
        case TOKEN::NUMBER:
            return _M_type::NUM;
        case TOKEN::NLBRAC:
        case TOKEN::NRBRAC:
            return _M_type::BRAC;
        default:
            return _M_type::OPERATOR;
    }
}

constexpr unsigned Level(TOKEN tk){
    switch (tk){
        case TOKEN::ADD:
        case TOKEN::NEG:
            return 3;
        case TOKEN::MULTI:
        case TOKEN::DIV:
            return 1;
        case TOKEN::VAR:
        case TOKEN::NUMBER:
            return 0;
        case TOKEN::AND:
        case TOKEN::NOT:
        case TOKEN::OR :
            return 2;
        default:
            return 1<<31;
    }
}


node::nodeptr parser(const unique_ptr<std::vector<vastina::token_t>>& tks, unsigned& offset){
    
    static BracketCount state;

    auto root = make_shared<node>(tks->at(offset));
    root->data.level = Level(root->data.tk.token);
    if(offset+1 >= tks->size()) return root;
    
    while(true){
        auto current = make_shared<node>(tks->at(offset));
        //_M_type type = _token_type(root->data.tk.token);
        offset++;
        switch (_token_type(current->data.tk.token)){
            case _M_type::BRAC:{
                if(current->data.tk.token == TOKEN::NRBRAC){    
                    if(++state.close > state.open) 
                        return nullptr;
                    root->data.level = 0;
                    return root;
                }else {
                    ++state.open;
                    if(_M_type::BRAC == _token_type(root->data.tk.token)) root = parser(tks, offset);
                    else current = parser(tks, offset);
                }
                break;
            }
            case _M_type::NUM:{
                if(root->data.tk.token == TOKEN::NUMBER) break;
                auto temp = root;
                while(temp->right != nullptr) temp = temp->right;
                //auto temp = root->FindChildR([](node::nodeptr _node){return _node->right == nullptr;});

                if(_M_type::OPERATOR != _token_type(temp->data.tk.token)) return nullptr;
                
                temp->right = current;
                current->parent = temp;

                break;
            }
            case _M_type::OPERATOR:{
                current->data.level = Level(current->data.tk.token); 
                if(current->data.level >= root->data.level){
                    root->ReplaceByLL(current);
                    root = current;
                }
                else{
                    auto temp = root;
                    while(temp->data.level > current->data.level){
                        if(temp->right == nullptr) break;
                        temp = temp->right;
                    }
                    if(temp != root)//current取代原node，原node成为current的left
                        temp->ReplaceByRL(current);
                    else
                        temp->InsertRight(current);
                }

                break;
            }

            default:
                return nullptr;
        }
        
        if(offset >= tks->size()) break;

    }

    return root;
}

int cal(const shared_ptr<node>& root){
    switch (root->data.tk.token)
    {
    case TOKEN::ADD:
        return cal(root->left) + cal(root->right);
        break;
    case TOKEN::NEG:
        return cal(root->left) - cal(root->right);
        break;
    case TOKEN::MULTI:
        return cal(root->left) * cal(root->right);
        break;
    case TOKEN::DIV:
        return cal(root->left) / cal(root->right);
        break;
    case TOKEN::NUMBER:
        return std::stoi(root->data.tk.data);
        break;
    case TOKEN::AND:
        return cal(root->left) & cal(root->right);
        break;
    case TOKEN::OR:
        return cal(root->left) | cal(root->right);
        break;
    default:
        return 0;
        break;
    }
}

int main(int argc, char* argv[]){
    // if(argc != 2){
    //     std::cout << "Usage: " << argv[0] << " <filename>\n";
    //     return 1;
    // }
    // lexer lx = lexer(argv[1]);

    lexer lx = lexer("./test/test");
    // (2+8)|5+3*7/4

    while (lexer::STATE::END != lx.Next()) ;

    auto tks = make_unique<std::vector<token_t>>(lx.getTokens());
    for(unsigned i=0; i < tks->size(); i++){
        std::cout << tks->at(i).token << ' ' << tks->at(i).data <<' '<< tks->at(i).line << '\n';
    }

    unsigned offset = 0;
    auto root = parser(tks, offset);
    if(root == nullptr){
        std::cout << "Error\n";
        return 1;
    }

    std::cout << '\n';
    root->Walk(walk_order::PREORDER, [](const _node& data_){
        std::cout  << data_.tk.data << '\n';
    });

    std::cout << cal(root) << '\n';

    return 0;
}