#include "symbol.hpp"
#include "expr.hpp"
#include "base/vasdef.hpp"
#include <iostream>

namespace vastina{

//get the child just created and add data in it
Scope::pointer Scope::CreateChild(range_t&& rr = {0,0}){
    children_.push_back(new Scope(this, std::move(rr)));
    return children_.back();
}
Scope::pointer Scope::getParent(){ return parent_; }

void Scope::addVar(const std::string_view& name, Variable&& var){st_.addVar(name, std::move(var));}
void Scope::addFunc(const std::string_view& name, Function&& fc){st_.addFunc(name, std::move(fc));}
Variable::pointer Scope::getVar(std::string_view name){
    auto node = this;
    do{
        auto res = node->st_.getVar(name);
        if(nullptr != res) return res;
        node = node->parent_;
    }while(nullptr != node);
    return nullptr;
}
Function::pointer Scope::getFunc(std::string_view name){
    auto node = this;
    do{
        auto res = node->st_.getFunc(name);
        if(nullptr != res) return res;
        node = node->parent_;
    }while(nullptr != node);
    return nullptr;
}
void Scope::setRange(unsigned start, unsigned end){r_.start=start;r_.end=end;}

bool Scope::varExist(const std::string_view& name){
    auto node = this;
    do{
        if(node->st_.varExist(name)) return true;
        else node = node->parent_;
    }while(nullptr != node);

    return false;
}

bool Scope::funcExist(const std::string_view& name){
    auto node = this;
    do{
        if(node->st_.funcExist(name)) return true;
        else node = node->parent_;
    }while(nullptr != node);

    return false;
}

inline TOKEN Preprocess::Current(){
   return primary_tokens[offset].token;
}

inline std::string_view Preprocess::CurrentTokenName(){
    return primary_tokens[offset].data;
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

int Preprocess::Except(TOKEN excepted, bool last=false){
    
//for debug
static int count = 0;
    if((Peek() != excepted)){
        if(last){
if(++count > 10) exit(0);
            //EXIT_ERROR
            std::cerr << __FILE__ <<' '<<__LINE__ <<'\n';
        }
        else return -1;
    }

    return 0;
}


int Preprocess::Process(){
    unsigned size =  primary_tokens.size();
    while (offset < size) {
        switch (Current()) {
            case TOKEN::SEMICOLON:
                Next();
                break;
            case TOKEN::OBRACE:{
                auto child = current_scope->CreateChild();
                current_scope = child;
                Next();
                break;
            }
            case TOKEN::CBRACE:{
                auto node = current_scope->getParent();
                current_scope = node;
                Next();
                break;
            }
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
                ProcessLoopType([]{return false;});
                break;
            case TOKEN::FUNC:
                ProcessCallType();
                break;
            case TOKEN::RETURN:
                ProcessRetType();
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

    //it should be the global scope now
    current_scope->setRange(0, getSize());
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

    std::function<void()> adder;
    switch (Current()) {
        //todo: varible need source location to init
        case TOKEN::INT:
            adder = [this](){current_scope->addVar(CurrentTokenName(), variable<int>());};
            break;
        case TOKEN::FLOAT:
            adder = [this](){current_scope->addVar(CurrentTokenName(), variable<float>());};
            break;
        case TOKEN::DOUBLE:
            
            break;
        default:
            return -1;
    }
    Next();

    unsigned last_offset = offset;
    while(true){
        if(Current()==TOKEN::SYMBOL){
            adder();
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
    if(0 != res) {RETURN_ERROR}

    return 0;
}

int Preprocess::ProcessLoopType(std::function<bool()> EndJudge){
    return {};
}

int Preprocess::ProcessParas(Function::pointer fc){



    return 0;
}
int Preprocess::ProcessCallType(){
    auto last_offset = offset;  Next();
    auto name = CurrentTokenName();
    if(Current()==TOKEN::MAIN){

    } //todo
    tryNextNext(TOKEN::COLON, true);//call it tryNextAndJump is better

    std::function<void()> adder;  
    switch (Current()) {
        case TOKEN::INT:
            adder = [this](){current_scope->addFunc(CurrentTokenName(), func<int>());};
            break;
        case TOKEN::FLOAT:
        case TOKEN::CHAR:
        //......
        default:
            THIS_NOT_SUPPORT(CurrentTokenName());
    }   adder();//adder or add?

    tryNext(TOKEN::NLBRAC, true);
    (void)ProcessParas(current_scope->getFunc(name));//no check here
    tryNextNext(TOKEN::NRBRAC, true);

    results.push_back({P_TOKEN::CALL, last_offset, offset});

    return 0;
}

int Preprocess::ProcessRetType(){
    results.push_back({P_TOKEN::RET, offset, offset+1});    Next();

    int res = ProcessCalType([this]{return Current()==TOKEN::SEMICOLON;});
    if(0 != res) {RETURN_ERROR}

    return 0;
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


Scope::pointer Preprocess::CurrentScope(){ return current_scope; }
} // namespace vastina
