#include "symbol.hpp"
#include "ast.hpp"
#include "base/vasdef.hpp"
#include <iostream>

namespace vastina{

template<typename  ty>
bool Variable<ty>::isSameName(std::string_view name){
    return this->name == name;
}

template<typename ty>
const ty& Variable<ty>::getValue(){
    return value;
}

inline TOKEN Preprocess::Current(){
   return primary_tokens[offset].token;
}

inline TOKEN Preprocess::Peek(){
    return primary_tokens[offset+1].token;
}

void Preprocess::Next(){
    offset++;
}

int Preprocess::Except(TOKEN excepted, bool last){
    if((Peek() != excepted)){
        if(last){
            std::cerr << __FILE__ <<' '<<__LINE__;
            exit(1);
        }
        else return -1;
    }
    return 0;
}


int Preprocess::Process(){
    while (true) {
        switch (Current()) {
            case TOKEN::IF:
                (void)Except(TOKEN::NLBRAC, true);
                Next();
                ProcessCalType([this](){ return Current() == TOKEN::OBRACE; });
                break;
            case TOKEN::SYMBOL:{
                int res = Except(TOKEN::ASSIGN, false);
                if(res == 0){
                    ProcessAssignType([this](){ return true;});
                    break;
                }
                else res = Except(TOKEN::SIGNED, true);
            }
            default:
                return -1;
        }
    }
}

//then I will make no check on the Parser of calculate because it is checked here
int Preprocess::ProcessCalType(std::function<bool()> EndJudge){

    BracketCount bc;

    while (true) {
        switch (Current()) {
// I need marhtable now
            case TOKEN::NLBRAC:{
                bc.open++;
                int  res = Except(TOKEN::NLBRAC, false);
                if(res == 0){
                    bc.open++; 
                    break;
                }
                else res = Except(TOKEN::SYMBOL, true);
            }
            case TOKEN::NRBRAC:{
                if(++bc.close > bc.open) return -1;
                auto next_token_type = cal_token_type(Peek());
                if(next_token_type == cal_type::OPERATOR){
                    break;
                }
                int  res = Except(TOKEN::NRBRAC, false);
                if(res == 0){
                    if(++bc.close > bc.open) return -1;
                    break;
                }
                else res = Except(TOKEN::OBRACE, false);
                if(res == 0){
                    break;
                }
                else res = Except(TOKEN::SEMICOLON, true);
            }
            case TOKEN::SYMBOL:{
                auto next_token_type = cal_token_type(Peek());
                if(next_token_type == cal_type::OPERATOR){
                    break;
                }
                else return -1;
                int  res = Except(TOKEN::NRBRAC, false);
                if(res == 0){
                    break;
                }
                else res = Except(TOKEN::SEMICOLON, true);
            }
            default:
                return -1;
        }

        Next();
    }
    return {};
}

int Preprocess::ProcessAssignType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessDeclType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessAddrType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessIfType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessGotoType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessCallType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessRetType(std::function<bool()> EndJudge){
    return {};
}


const Preprocess::p_token_t& Preprocess::getNext(){
    if(offset >= results.size()){
        //todo: log it
        exit(1);
    }
    return static_cast<const p_token_t&>(results[id++]);
}

const unsigned Preprocess::getSize() const{
    return results.size();
}

} // namespace vastina
