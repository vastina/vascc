#include "symbol.hpp"

namespace vastina{

template<typename  ty>
bool Variable<ty>::isSameName(std::string_view name){
    return this->name == name;
}

template<typename ty>
const ty& Variable<ty>::getValue(){
    return value;
}


int Preprocess::Process(){
    return {};
}

int Preprocess::ProcessCalType(std::function<bool()> EndJudge){
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
    return static_cast<const p_token_t&>(results[offset++]);
}

const unsigned Preprocess::getSize() const{
    return results.size();
}

} // namespace vastina
