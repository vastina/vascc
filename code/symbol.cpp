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
            std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
            //exit(1);
        }
        else return -1;
    }
    return 0;
}

//temporarily use ca;_type here
int Preprocess::Process(){
    unsigned size =  primary_tokens.size();
    while (offset <= size) {
        switch (Current()) {
            case TOKEN::IF:
                results.push_back({P_TOKEN::IF, offset, offset+1});
                (void)Except(TOKEN::NLBRAC, true);
                Next();
                ProcessCalType([this](){ return Current() == TOKEN::OBRACE; });
                break;
            case TOKEN::SYMBOL:{
                int res = Except(TOKEN::ASSIGN, false);
                if(res == 0){
                    ProcessAssignType([](){ return false;});
                    break;
                }
                else res = Except(TOKEN::SIGNED, true);
                break;
            }
            default:{
                switch (cal_token_type(Current())) {
                    case cal_type::TYPE:{
                        (void)Except(TOKEN::SYMBOL, true);
                        ProcessDeclType([this](){ return Current() == TOKEN::SEMICOLON;});
                        break;
                    }


                    default: 
                        std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
                        return -1;
                }
            }
        }
    }
    return 0;
}

//then I will make no check on the Parser of calculate because it is checked here
int Preprocess::ProcessCalType(std::function<bool()> EndJudge){

    BracketCount bc;
    unsigned last_offset = offset;

    while (true) {
        switch (cal_token_type(Current())) {
            case cal_type::BRAC:{
                if(Current() == TOKEN::NRBRAC){
                    if(++bc.close > bc.open) return -1;
                }
                else{
                    ++bc.open;
                }
                break;
            }
            case cal_type::VALUE:{
                if(cal_token_type(Peek()) == cal_type::VALUE){
                    std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
                    return -1;
                }
                break;
            }
            case cal_type::OPERATOR:{
                auto peek = Peek();
                if(cal_token_type(peek) == cal_type::OPERATOR){
                    if(peek!=TOKEN::NEG && peek!=TOKEN::ADD && peek!=TOKEN::OPS && peek!=TOKEN::LOGNOT){
                        std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
                        return -1;
                    }
                }
                break;
            }
            default:
                std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
                return -1;
        }
        Next();
        if(EndJudge()) break;
    }

    results.push_back({P_TOKEN::CAL, last_offset, offset});
    return 0;
}

int Preprocess::ProcessAssignType(std::function<bool()> EndJudge){

    unsigned last_offset = offset;

    while(true){
        if(Current()==TOKEN::SYMBOL && Peek()==TOKEN::ASSIGN){
            Next(); Next();
        }
        else{
            results.push_back({P_TOKEN::ASSIGN, last_offset, offset});
            (void)ProcessCalType([this](){return Current()==TOKEN::SEMICOLON || Current()==TOKEN::EQUAL;});
            break;
        }
        if(EndJudge()) break;
    }

    return 0;
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
    if(id >= results.size()){
        //todo: log it
        exit(1);
    }
    return static_cast<const p_token_t&>(results[id++]);
}

const unsigned Preprocess::getSize() const{
    return results.size();
}

} // namespace vastina
