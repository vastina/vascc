#include "base/vasdef.hpp"
#include "lexer.hpp"

#include <iostream>
#include <memory>

using namespace vastina;

using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

typedef struct node{
    token_t data;
    unsigned level = 0;
    shared_ptr<struct node> left;
    shared_ptr<struct node> right;
    shared_ptr<struct node> parent;

    node(): data(TOKEN::UNKNOW), left(nullptr), right(nullptr), parent(nullptr){};
    node(token_t tk): data(tk), left(nullptr), right(nullptr), parent(nullptr){};
} node;
    
// enum STATE{
//     INSIDE, //open  bracket more than close bracket
//     OUTSIDE,//close bracket equal to  open bracket
//     ERROR   //close bracket more than open bracket
// };
typedef struct State{
    unsigned open = 0;
    unsigned close = 0;
} State;

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

void walk(const shared_ptr<node>& root){
    if(root->left != nullptr){
        std::cout << "left ";
        walk(root->left);
    }
    std::cout << root->data.data << '\n';
    if(root->right != nullptr){
        std::cout << "right ";
        walk(root->right);
    }
    
}

shared_ptr<node> parser(const unique_ptr<std::vector<vastina::token_t>>& tks, unsigned& offset){
    
    static State state;

    auto root = make_shared<node>(tks->at(offset));
    root->level = Level(root->data.token);
    if(offset+1 >= tks->size()) return root;
    

    while(true){
        auto current = make_shared<node>(tks->at(offset));
        //_M_type type = _token_type(root->data.token);
        switch (_token_type(current->data.token)){
            case _M_type::BRAC:{
                if(current->data.token == TOKEN::NRBRAC){
                    offset++;
                    if(++state.close > state.open){
                        std::cout << state.open <<' ' << state.close <<'\n';
                        return nullptr;
                    }
                    root->level = 0;
                    return root;
                }else {
                    ++state.open;
                    ++offset;
                    if(_M_type::BRAC == _token_type(root->data.token)) root = parser(tks, offset);
                    else current = parser(tks, offset);
                }
                break;
            }
            case _M_type::NUM:{
                if(root->data.token == TOKEN::NUMBER){
                    offset++;
                    break;
                }

                auto temp = root;
                if(root->right != nullptr)
                    temp = root->right;

                while(temp->right != nullptr) temp = temp->right;

                if(_M_type::OPERATOR != _token_type(temp->data.token)) return nullptr;
                
                temp->right = current;
                current->parent = temp;

                offset++;
                break;
            }
            case _M_type::OPERATOR:{
                current->level = Level(current->data.token); 
                if(current->level >= root->level){
                    current->left = root;
                    root->parent = current;
                    root = current;
                }
                else{
                    auto temp = root;
                    while(temp->level > current->level){
                        if(temp->right == nullptr) break;
                        temp = temp->right;
                    }
                    if(temp != root){
                        temp->parent->right = current;
                        current->parent = temp->parent;
                        temp->parent = current;
                        current->left = temp;
                        //current取代现在的node
                    }
                    else{
                        temp->right = current;
                        current->parent = temp;
                    }
                    
                }

                offset++;

                break;
            }

            default:
                return nullptr;
        }
        
        if(offset >= tks->size()) break;

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
    walk(root);

    return 0;
}