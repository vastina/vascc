#ifndef _SYMBOL_H_
#define _SYMBOL_H_

//#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "lexer.hpp"

#include <string_view>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

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
    using pointer = Variable*;

    Variable(): isConst_(false) {};
    ~Variable()=default;
    inline bool isConst(){return isConst_;};
    inline pointer self(){return this;}
};

template<typename ty>
class variable :public Variable{
private:
    //ty value_;
    //I need source_location
public:
    using Variable::Variable;
    using Variable::isConst_;
    using Variable::isConst;

    variable(){};

    // inline const ty& getValue_ref(){ return value_;}
    // inline const ty getValue_copy(){ return value_;}
    // inline void setValue(const ty& v){ value_ = v;}
    // inline void setValue(ty&& v){ value_ = std::move(v);}
    // I don't need the fucking stupid getter and setter
};

class Function{
protected:
    bool isVoid_{};
public:
    using pointer = Function*;

    ~Function() = default;
    inline pointer self(){return this;}
};

template<typename ty>
class func :public Function{
public:
    ;
private:
    ;
};


//so this is global, sington is for java
//std::unordered_map<std::string_view, Variable> Global_Variable;
//ok, I won't use this, I will use a class to manage this

typedef struct SymbolTable{
    std::unordered_map<std::string_view, Variable> Variables;
    std::unordered_map<std::string_view, Function> functions;

    inline bool varExist(std::string_view name){return Variables.count(name);}
    inline bool funcExist(std::string_view name){return functions.count(name);}
    inline void addVar(const std::string_view& name, Variable&& var){
        Variables.insert(std::make_pair(name, var)); }
    inline Variable::pointer getVar(std::string_view name){
        if(Variables.count(name)) return Variables.at(name).self();
        return nullptr;
    }
    inline Function::pointer getFunc(std::string_view name){
        if(Variables.count(name)) return functions.at(name).self();
        return nullptr;
    }
    // inline Variable::pointer getVar( `the source-location` )
    inline void addFunc(const std::string_view& name, Function&& fc){
        functions.insert(std::make_pair(name, fc));  }
} SymbolTable;



typedef struct range_t{
    unsigned start; unsigned end;//[start, end) preprocessed_tokens[start] to preprocessed_tokens[end-1]
    // range_t* parent;    
    // range_t* next;    range_t* child;

    range_t(): start(0), end(0){};
    inline const range_t& operator =(range_t&& other){
        start=other.start; end=other.end;
        return *this;
    }
    range_t(unsigned st, unsigned ed):start(st), end(ed){};
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

    using pointer = Scope*;
    using nodeptr = std::shared_ptr<Scope>;

private:

    pointer parent_; //if can't find symbol, ask parent
    range_t r_;
    SymbolTable st_; //so decl a samename-var in child scope is acceptable
    std::vector<pointer> children_;
public:
    Scope() = delete;
    Scope(range_t&& r): parent_(nullptr), r_(r), st_(), children_() {};
    Scope(pointer parent, range_t&& r): parent_(parent), r_(r), st_(), children_() {};

    pointer CreateChild(range_t&&);
    pointer getParent();

    void addVar(const std::string_view& name, Variable&& var);
    void addFunc(const std::string_view& name, Function&& fc);
    Variable::pointer getVar(std::string_view name);
    Function::pointer getFunc(std::string_view name);
    void setRange(unsigned start, unsigned end);

    bool varExist(const std::string_view& name);
    bool funcExist(const std::string_view& name);
};

class Preprocess{
public:
    enum P_TOKEN{//processed token
        UNKNOW = -1,
        CAL, ASSIGN, DECL, ADDR,
        IF, LOOP, CALL, RET,
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

    Scope::pointer current_scope;
public:
    Preprocess(std::vector<token_t>& tks): primary_tokens(tks), offset(), current_scope(new Scope({0,0})) {};
    ~Preprocess() = default;
private:
    inline TOKEN Current();
    inline std::string_view CurrentTokenName();
    inline TOKEN Peek(unsigned _offset);
    inline TOKEN PreToken(unsigned _offset);
    void Next();
    //last==true means if this don't match, stop and return error
    int Except(TOKEN excepted, bool last);
    #define tryNext(excepted ,last) \
        do{ \
            if(Except(excepted, last) == 0){ Next(); }\
            else {RETURN_ERROR} \
    }while(0)
    #define tryNextNext(excepted ,last) \
        do{ \
            if(Except(excepted, last) == 0){ Next(); Next(); }\
            else {RETURN_ERROR} \
    }while(0)

private:
    //someone need custom judge

    //please be careful about Cal and Call, so familar
    int ProcessCalType(std::function<bool()> EndJudge);
    int ProcessAssignType(std::function<bool()> EndJudge);
    int ProcessDeclType(std::function<bool()> EndJudge);
    int ProcessAddrType(std::function<bool()> EndJudge);
    int ProcessIfType();
    int ProcessLoopType(std::function<bool()> EndJudge);
    int ProcessRetType();
    
    int ProcessParas(Function::pointer fc);
    int ProcessCallType();
//return a code to indicate the result
public:
    int Process();
    const p_token_t& getNext();
    const unsigned getSize() const;

    //for test?
    Scope::pointer CurrentScope();
};




}//namespace vastina


#endif