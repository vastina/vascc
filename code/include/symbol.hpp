#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "base/vasdef.hpp"
#include "lexer.hpp"

#include <string_view>
#include <vector>
#include <functional>

namespace vastina{

// "create symbol tables for every scope"

typedef struct range{
    unsigned start; unsigned end;//[start, end) preprocessed_tokens[start] to preprocessed_tokens[end-1]
    // range* parent;    
    // range* next;    range* child;//child指向的是第一个子scope，next指向的是下一个兄弟scope

    range(): start(0), end(0){};
    range(unsigned s, unsigned e): start(s), end(e){};
    inline bool isInRange(unsigned index){
        return index >= start && index < end;
    }
    inline bool isInRange(range* r){
        return r->start >= start && r->end <= end;
    }

} range;

class Scope{
public:
    using pointer = Scope*;

private:
    pointer parent;
    pointer next;
    pointer child; 


};

class Preprocess{
public:
    enum P_TOKEN{//processed token
        UNKNOW = -1,
        CAL, ASSIGN, DECL, ADDR,
        IF, GOTO, CALL, RET,
    };
    
    typedef struct p_token_t{
        P_TOKEN tk;
        unsigned start;
        unsigned end;
    } p_token_t;
    //[start, end)
    //这个是后来写的，在expression后面加进来的，也许以后会把expr class里面的food_改成和这个更匹配

private:
    std::vector<token_t>& primary_tokens;
    unsigned offset;
    
    std::vector<p_token_t> results;
    unsigned id = 0;

    Scope* current_scope;
public:
    Preprocess(std::vector<token_t>& tks): primary_tokens(tks), offset(), current_scope() {};
    ~Preprocess() = default;
private:
    inline TOKEN Current();
    inline TOKEN Peek(unsigned _offset);
    inline TOKEN PreToken(unsigned _offset);
    void Next();
    int Except(TOKEN excepted, bool last);
private:
    int ProcessCalType(std::function<bool()> EndJudge);
    int ProcessAssignType(std::function<bool()> EndJudge);
    int ProcessDeclType(std::function<bool()> EndJudge);
    int ProcessAddrType(std::function<bool()> EndJudge);
    int ProcessIfType();
    int ProcessGotoType(std::function<bool()> EndJudge);
    int ProcessCallType(std::function<bool()> EndJudge);
    int ProcessRetType(std::function<bool()> EndJudge);
//return a code to indicate the result
public:
    int Process();
    const p_token_t& getNext();
    const unsigned getSize() const;
};





template<typename ty>
class Literal{//compile time values like "Hello World",114514,3.14
private:
    ty value;
public:
    Literal() = default;
    ~Literal() = default;
};

template<typename ty>
class Variable{
private:
    ty value;
    std::string_view name;

public:
    Variable(ty v,std::string_view n): value(v),name(n){};
    
    bool isSameName(std::string_view name);
    const ty& getValue();
};

typedef struct SymbolTable{

} SymbolTable;




}//namespace vastina


#endif