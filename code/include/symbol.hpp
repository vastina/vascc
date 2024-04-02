#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "base/vasdef.hpp"
#include "lexer.hpp"

#include <string_view>
#include <vector>
#include <functional>

namespace vastina{

// "add symbol tables for every scope"


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
public:
    Preprocess(std::vector<token_t>& tks): primary_tokens(tks){};
    ~Preprocess() = default;
private:
    inline TOKEN Current();
    inline TOKEN Peek();
    void Next();
    int Except(TOKEN excepted, bool last);
private:
    int ProcessCalType(std::function<bool()> EndJudge);
    int ProcessAssignType(std::function<bool()> EndJudge);
    int ProcessDeclType(std::function<bool()> EndJudge);
    int ProcessAddrType(std::function<bool()> EndJudge);
    int ProcessIfType(std::function<bool()> EndJudge);
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


class Scope{

};


}//namespace vastina


#endif