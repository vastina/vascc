#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "lexer.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>
#include <functional>

namespace vastina{

// "create symbol tables for every scope"

template<typename ty>
class Literal{//compile time values like "Hello World",114514,3.14
private:
    ty value;
public:
    Literal() = default;
    ~Literal() = default;
};


class Variable{
protected:
    bool isConst_;
public:
    Variable(): isConst_(false) {};
    ~Variable()=default;
    inline bool isConst(){return isConst_;};
};

template<typename ty>
class variable :public Variable{
private:
    ty value_;
public:
    using Variable::Variable;
    using Variable::isConst_;
    using Variable::isConst;
    inline const ty& getValue_ref(){ return value_;}
    inline const ty getValue_copy(){ return value_;}
    inline void setValue(const ty& v){ value_ = v;}
    inline void setValue(ty&& v){ value_ = std::move(v);}
};

//so this is global, sington is for java
//std::unordered_map<std::string_view, Variable> Global_Variable;
//ok, I won't use this, I will use a class to manage this

typedef struct SymbolTable{
    std::unordered_map<std::string_view, Variable> Variables;
    //std::unordered_map<std::string_view, > functions

    inline bool varExist(std::string_view name){return Variables.count(name);}
} SymbolTable;



typedef struct range_t{
    unsigned start; unsigned end;//[start, end) preprocessed_tokens[start] to preprocessed_tokens[end-1]
    // range_t* parent;    
    // range_t* next;    range_t* child;

    range_t(): start(0), end(0){};
    range_t(range_t&& other): start(other.start), end(other.end){};
    range_t(const range_t& other): start(other.start), end(other.end){};
    inline bool isInRange(unsigned index){
        return index >= start && index < end;
    }
    inline bool isInRange(range_t* r){
        return r->start >= start && r->end <= end;
    }

} range_t;

// "create symbol tables for every scope"
class Scope{
public:
    typedef struct _scope_node{
        range_t r_;
        SymbolTable st_;
        
        _scope_node(const range_t& r): r_(r), st_(){};
        _scope_node(range_t&& r): r_(std::move(r)), st_(){};
        //其实没必要，现在的rang_t只剩两个不拥有资源的unsigned了
    } _scope_node;

    typedef TreeNode<_scope_node> scope_node;

private:
    scope_node root_;// left is child, right is next
    
    // range_t r_;
    // SymbolTable st_; //so decl a samename-var in child scope is acceptable
    // pointer parent_; //if can't find symbol, ask parent
    // pointer next_;
    // pointer child_; //child指向的是第一个子scope，next指向的是下一个兄弟scope
public:
    // Scope() = delete;
    // Scope(range_t&& r): r_(std::move(r)), st_(), parent_(nullptr), next_(nullptr), child_(nullptr){};
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




}//namespace vastina


#endif