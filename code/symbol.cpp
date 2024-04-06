#include "symbol.hpp"
#include "expr.hpp"
#include "base/vasdef.hpp"
#include <iostream>

namespace vastina{


inline TOKEN Preprocess::Current(){
   return primary_tokens[offset].token;
}

inline TOKEN Preprocess::Peek(unsigned _offset=1){
    return primary_tokens[offset+_offset].token;
}

inline TOKEN Preprocess::PreToken(unsigned _offset=1){
    if(_offset>offset) return TOKEN::UNKNOW;
    return primary_tokens[offset-_offset].token;
}

void Preprocess::Next(){
    offset++;
}

int Preprocess::Except(TOKEN excepted, bool last){
    if((Peek() != excepted)){
        if(last){
            //EXIT_ERROR
            std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
        }
        else return -1;
    }
    return 0;
}

//temporarily use ca;_type here
int Preprocess::Process(){
    unsigned size =  primary_tokens.size();
    while (offset < size) {
        switch (Current()) {
            case TOKEN::SEMICOLON:
                Next();
                break;
            case TOKEN::CBRACE:
            //this is a scope end, do something
                Next();
                break;
            case TOKEN::IF:
                ProcessIfType();
                break;
            case TOKEN::SYMBOL:{
                int res = Except(TOKEN::ASSIGN, false);
                if(res == 0){
                    ProcessAssignType([this](){ return Current()==TOKEN::SEMICOLON || Current()==TOKEN::COMMA;});
                    break;
                }
                else res = Except(TOKEN::SEMICOLON, true);
                break;
            }
            case TOKEN::WHILE:
            case TOKEN::FOR:
            case TOKEN::DO:
                ProcessGotoType([]{return false;});
                break;
            case TOKEN::SYMBOLF:
                ProcessCallType([]{return false;});
                break;
            case TOKEN::RETURN:
                ProcessRetType([]{return false;});
                break;
            default:{
                switch (token_type(Current())) {
                    case TOKEN_TYPE::TYPE:{
                        (void)Except(TOKEN::SYMBOL, true);
                        ProcessDeclType([this](){ return Current() == TOKEN::SEMICOLON;});
                        break;
                    }


                    default: 
                        std::cout << Current() <<'\n';
                        {RETURN_ERROR}
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
        switch (token_type(Current())) {
            case TOKEN_TYPE::BRAC:{
                if(Current() == TOKEN::NRBRAC){
                    if(++bc.close > bc.open) return -1;
                }
                else{
                    ++bc.open;
                }
                break;
            }
            case TOKEN_TYPE::VALUE:{
                if(token_type(Peek()) == TOKEN_TYPE::VALUE){
                    {RETURN_ERROR}
                }
                break;
            }
            case TOKEN_TYPE::OPERATOR:{
                auto peek = Peek();
                if(token_type(peek) == TOKEN_TYPE::OPERATOR){
                    if(peek!=TOKEN::NEG && peek!=TOKEN::ADD && peek!=TOKEN::OPS && peek!=TOKEN::LOGNOT){
                        {RETURN_ERROR}
                    }
                }
                break;
            }
            default:
                {RETURN_ERROR}
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
            (void)ProcessCalType([this](){return Current()==TOKEN::SEMICOLON;});
            break;
        }
        if(EndJudge()) break;
    }

    return 0;
}

int Preprocess::ProcessDeclType(std::function<bool()> EndJudge){

    switch (Current()) {
        case TOKEN::INT:
            
            break;
        case TOKEN::FLOAT:

            break;
        default:
            return -1;
    }
    Next();

    unsigned last_offset = offset;
    while(true){
        if(Current()==TOKEN::SYMBOL){
            //add something to symbol table
            Next();
        }
        else if(Current()==TOKEN::ASSIGN){
            results.push_back({P_TOKEN::ASSIGN, last_offset, offset+1});
            last_offset = offset+1;
            Next();
            (void)ProcessCalType([this](){return Current()==TOKEN::SEMICOLON || Current()==TOKEN::COMMA;});
        }
        else if(Current()==TOKEN::COMMA){
            results.push_back({P_TOKEN::ASSIGN, last_offset, offset});
            last_offset = offset+1;
            Next();
        }
        else if(EndJudge()) break;
    }

    return 0;
}

int Preprocess::ProcessAddrType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessIfType(){
    results.push_back({P_TOKEN::IF, offset, offset+1});
    (void)Except(TOKEN::NLBRAC, true);  Next();
    int res = ProcessCalType([this](){ return Current() == TOKEN::OBRACE; });//暂时不支持不带{}的if
    if(0 == res) Next();
    else {RETURN_ERROR}
    
    // notify some class there is a scope, 
    // variables declared in this scope should be added to the symbol table
    // and all can know the scope of variables declared here

    //(void)Except(TOKEN::CBRACE, true); Next();
    return 0;
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
