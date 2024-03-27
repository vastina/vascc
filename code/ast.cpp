#include "ast.hpp"

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

} //namespace vastina