#include "ast.hpp"
#include "base/vasdef.hpp"

#include <iostream>

namespace vastina{

/*----------------------------------matchtable----------------------------------------------*/

bool MatchTable::MatchUnit::operator==(const MatchUnit& m) const{
    return tk_type == m.tk_type && current_state == m.current_state;
}

bool MatchTable::MatchUnit::operator!=(const MatchUnit& m) const{
    return !(this->operator==(m));
}

MatchTable::MatchTable() = default;
MatchTable::~MatchTable(){};

void MatchTable::RegisteCase(TOKEN current_token, TOKEN except_token, FSM state){
    auto& matcher = matchTable[current_token];
    matcher.units.push_back({except_token, state});
}

void MatchTable::UnregisteCase(TOKEN current_token, TOKEN except_token, FSM state){
    auto& matcher = matchTable[current_token];
    for(auto it = matcher.units.begin(); it != matcher.units.end(); it++){
        if(it->tk_type == except_token && it->current_state == state){
            matcher.units.erase(it);
            return;
        }
    }
}

void MatchTable::Error(const char* what){
    std::cerr << what << std::endl;
    exit(1);
}//maybe I should use some logger

void MatchTable::Warn(const char* what){
    std::cerr << what << std::endl;
}


const std::vector<MatchTable::MatchUnit>& MatchTable::getMatchList(TOKEN tk){
    return matchTable[tk].units;
}

TOKEN MatchTable::getNextMarch(TOKEN current_token){
    auto& matcher = matchTable[current_token];
    if(matcher.offset >= matcher.units.size()){
        Warn("no match found");
        return TOKEN::UNKNOW;
    }
    return matcher.units[matcher.offset++].tk_type;
}

/*----------------------------------matchtable----------------------------------------------*/


/*----------------------------------parser-------------------------------------------------*/

Parser::Parser(std::vector<token_t>& tks):tokens(tks){};

inline token_t& Parser::Peek(){
    if(offset >= tokens.size()) return tokens[-1];
    return tokens[offset+1];
}

void Parser::Eat(){
    switch (current_state){
        case FSM::START:
            
            break;
        case FSM::DECL:

            break;
        case FSM::ASSI:

            break;
        case FSM::CAL:

            break;
        case FSM::GOTO:

            break;
        default:
            MatchTable::getInstance().Error("no such state");
    
    }
}

void Parser::Except(const token_t& excepted){
    if(Peek().token != excepted.token) {
        std::cout << "excepted " << excepted.data << " but got " << Peek().data << std::endl;
        std::cout << "at line " << Peek().line << std::endl;
        exit(1);
    }
}

void Parser::TryEat(const token_t& excepted){
    if(Peek().token == excepted.token) {
        Eat();
        offset++;
    }
}

void Parser::Stmt(Statement& stmt){

}

/*----------------------------------parser----------------------------------------------*/

template<typename ty>
void baseExpression<ty>::Walk(walk_order)const{}

template<typename ty>
void baseExpression<ty>::Parse(){}

template<typename ty>
const ty baseExpression<ty>::getValue(){
    return value_;
}

template<typename ty>
void baseExpression<ty>::setValue(const ty& v){
    value_ = v;
}

template<typename ty>
void baseExpression<ty>::setValue(ty&& v){
    value_ = std::move(v);
}



ExpressionUnit::ExpressionUnit(TokenPtr tks, unsigned s, unsigned e): tokens(tks), start(s), end(e){};

template<typename ty>
CalExpression<ty>::CalExpression(ExpressionUnit&& e):food_(std::move(e)), root_(nullptr){};

template<typename ty>
void CalExpression<ty>::Walk(walk_order wo) const{
    root_->Walk(wo, [](const _cal_node& data_){
        std::cout  << data_.tk.data  << '\n';
    });
}

template<typename ty>
void CalExpression<ty>::Parse(){
    unsigned offset = food_.start;
    root_ = Parse_(offset);
}

template<typename ty>
typename CalExpression<ty>::cal_node::pointer CalExpression<ty>::Parse_(unsigned &offset){

    static BracketCount bc;

    auto root = new cal_node(food_.tokens->at(offset));
    root->data.level = Level(root->data.tk.token);
    if(offset >= food_.end) return root;
    
    while(true){
        auto current = new cal_node(food_.tokens->at(offset));
        offset++;
        switch (cal_token_type(current->data.tk.token)){
            case cal_type::BRAC:{
                if(current->data.tk.token == TOKEN::NRBRAC){    
                    if(++bc.close > bc.open) 
                        return nullptr;
                    root->data.level = 0;
                    return root;
                }else {
                    ++bc.open;
                    if(cal_type::BRAC == cal_token_type(root->data.tk.token)) root = Parse_(offset);
                    else current = Parse_(offset);
                }
                break;
            }
            case cal_type::VALUE:{
                if(root->data.tk.token == TOKEN::NUMBER) break;

                auto temp = root->FindChildR(
                    [](const typename cal_node::pointer _node) {return (_node->right == nullptr);}
                );

                if(cal_type::OPERATOR != cal_token_type(temp->data.tk.token)) return nullptr;
                
                temp->InsertRight(current);

                break;
            }
            case cal_type::OPERATOR:{
                current->data.level = Level(current->data.tk.token); 
                if(current->data.level >= root->data.level){
                    root->ReplaceByL(current);
                    root = current;
                }
                else{
                    auto temp = root->FindChildR(
                        [&current](const typename cal_node::pointer _node)->bool {
                            return (_node->right == nullptr)||(_node->data.level <= current->data.level);
                    });
                    if(temp != root)//current取代原node，原node成为current的left
                        temp->ReplaceByL(current);
                    else
                        temp->InsertRight(current);
                }

                break;
            }

            default:
                return nullptr;
        }
        
        if(offset >= food_.end) break;

    }

    return root;

}

template<typename ty>
void CalExpression<ty>::Calculate(){
    value_ = Calculate_(root_);
    return;
}

template<>
int CalExpression<int>::Calculate_(const typename cal_node::pointer root){
    switch (root->data.tk.token) {
        case TOKEN::ADD:
            return Calculate_(root->left) + Calculate_(root->right);
            break;
        case TOKEN::NEG:
            return Calculate_(root->left) - Calculate_(root->right);
            break;
        case TOKEN::MULTI:
            return Calculate_(root->left) * Calculate_(root->right);
            break;
        case TOKEN::DIV:
            return Calculate_(root->left) / Calculate_(root->right);
            break;
        case TOKEN::NUMBER:
            return std::stoi(root->data.tk.data);;
            break;
        case TOKEN::AND:
            return Calculate_(root->left) & Calculate_(root->right);
            break;
        case TOKEN::OR:
            return Calculate_(root->left) | Calculate_(root->right);
            break;
        default:
            return 0;
            break;
    }
}


//除了显式实例化导出模板，还有更优雅一点的方法吗。。。
template class baseExpression<int>;
template class CalExpression<int>;

} //namespace vastina