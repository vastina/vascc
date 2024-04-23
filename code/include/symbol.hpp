#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "base/vasdef.hpp" //for the fucking stupid and smart clangd, includeit directly
#include "base/String.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <folly/Function.h>

namespace vastina {
struct token_t {
    TOKEN token;
    std::string_view name;
    u32 line;

    // some were here just because I am lazy to delete them
    token_t(TOKEN tk);
    token_t(TOKEN tk, const std::string_view &sv);
    token_t(TOKEN tk, std::string_view &&sv);
    token_t(TOKEN tk, const std::string_view &sv, u32 _line);
    token_t(TOKEN tk, std::string_view &&sv, u32 _line);

    token_t(const token_t &tk);
    token_t(token_t &&tk);
};
// "create symbol tables for every scope"

using SourceLocation = token_t;
class Value {

  public:
    using pointer = Value *;
    const SourceLocation &Srcloc_;
    Value(const SourceLocation &srcloc) : Srcloc_(srcloc){};
    ~Value() = default;
    
};

class Literal : public Value {
  protected:
    bool isTrivial{};
};

template <typename ty>
class literal : public Literal { // compile time values like "Hello World",114514,3.14
  private:
    ty value;

  public:

    literal() = default;
    ~literal() = default;

    inline std::string_view
    Typename() {
        return typeid(ty).name();
    }
};


class Variable : public Value {
  public:
    using pointer = Variable *;    

  protected:
    bool isConstexpr_{};
    bool isTrivial_{};
    

  public:
    Variable() = delete;
    Variable(const SourceLocation &srcloc) : Value(srcloc){};
    ~Variable() = default;
    inline bool
    isConst() {
        return isConstexpr_;
    };
    inline pointer
    self() {
        return this;
    }
};

template <typename ty>
class variable : public Variable {
  protected:
    // I need reflect or a member to mark the type
  public:

    variable(const SourceLocation &srcloc) : Variable(srcloc){};

    inline constexpr TOKEN Type() { return Type<ty>(); };
};

class Function {
  protected:
    bool isVoid_{};
    std::vector<Variable::pointer> paras_{};
    const SourceLocation &Srcloc_;

  public:
    using pointer = Function *;

    Function(const SourceLocation &Srcloc) : Srcloc_(Srcloc) { };
    ~Function() { paras_.clear(); };
    inline pointer
    self() {
        return this;
    }

    virtual TOKEN Type(){return TOKEN::UNKNOW;};
    virtual u32 getParamSize(){return {};};
    std::string_view getName(){return Srcloc_.name;};
};

template <typename ty>
class func : public Function {
  public:
    func(const SourceLocation &Srcloc) : Function(Srcloc){};

    inline constexpr TOKEN Type()override { return ::vastina::Type<ty>(); };
    ty RetureType()const {};

  private:
    ;
};

typedef struct SymbolTable {
    std::unordered_map<std::string_view, Variable> Variables;
    std::unordered_map<std::string_view, Function> functions;

    inline bool
    varExist(const std::string_view &name) const{
        return static_cast<bool>(Variables.count(name));
    }
    inline bool
    funcExist(const std::string_view &name) const{
        return static_cast<bool>(functions.count(name));
    }
    // Variables.insert(std::make_pair(name, var)); }
    inline Variable::pointer
    getVar(std::string_view name) {
        if (Variables.count(name))
            return Variables.at(name).self();
        return nullptr;
    }
    inline Function::pointer
    getFunc(std::string_view name) {
        if (Variables.count(name))
            return functions.at(name).self();
        return nullptr;
    }

    // always override
    inline void addVar(const std::string_view &name, const Variable &var) {
        Variables.erase(name);
        Variables.insert(std::make_pair(name, var));
        // Variables[name] = var;
    }
    // always override
    inline void addFunc(const std::string_view &name, const Function &fc) {
        functions.erase(name);
        functions.insert(std::make_pair(name, fc));
        // functions[name] = fc;
    }

    // inline Variable::pointer getVar( `the source-location` ) todo
} SymbolTable;

typedef struct range_t {
    u32 start;
    u32 end; //[start, end) preprocessed_tokens[start] to
             // preprocessed_tokens[end-1]

    range_t() : start(0), end(0){};
    inline const range_t &
    operator=(range_t &&other) {
        start = other.start;
        end = other.end;
        return *this;
    }
    range_t(u32 st, u32 ed) : start(st), end(ed){};
    range_t(range_t &&other) : start(other.start), end(other.end){};
    range_t(const range_t &other) : start(other.start), end(other.end){};
    inline bool
    isInRange(u32 index) {
        return index >= start && index < end;
    }
    inline bool
    isInRange(range_t *r) {
        return r->start >= start && r->end <= end;
    }

} range_t;

// "create symbol tables for every scope"
class Scope {
  public:
    using pointer = Scope *;
    using ScopePtr = std::shared_ptr<Scope>;

  private:
    pointer parent_; // if can't find symbol, ask parent
    range_t r_;
    SymbolTable st_; // so decl a samename-var in child scope is acceptable
    std::vector<pointer> children_;

    bool isBreakable_{false}; // for loop, who else need this?

  public:
    Scope() = delete;
    Scope(range_t &&r) : parent_(nullptr), r_(r), st_(), children_(){};
    Scope(pointer parent, range_t &&r)
        : parent_(parent), r_(r), st_(), children_(){};

    void addVar(const std::string_view &, const Variable &);
    void addFunc(const std::string_view &, const Function &);
    Variable::pointer getVar(const std::string_view &);
    Function::pointer getFunc(const std::string_view &);
    bool varExist(const std::string_view &);
    bool funcExist(const std::string_view &);

    void setRange(u32, u32);
    const range_t &getRange();
    range_t findRange(u32);
    //range_t getNextRangeBetweenChildren(); //this is too stupid, I won't do that
    void setBreakable(bool);
    inline void reSet(){idchild_ = 0;};

    // for test
    const decltype(children_) &getChildren();
    SymbolTable &getSymbolTable();

    pointer CreateChild(range_t &&);
    pointer getNextChild();
    pointer getChildat(u32);
    pointer getParent();
    pointer getRoot();
    
    // so bad
    u32 idchild_{};
};

typedef struct p_token_t {
    P_TOKEN tk;
    u32 start;
    u32 end;

    inline void setRang(u32 _start, u32 _end){
        start = _start;
        end = _end;
    }
} p_token_t;
//[start, end)
//use rang_t here maybe better

class Preprocess {
  public:
    using pointer = Preprocess *;

  private:
    std::vector<token_t> &primary_tokens;
    u32 offset;

    std::vector<p_token_t> results;
    u32 id = 0;

    Scope::pointer current_scope;

  public:
    Preprocess(std::vector<token_t> &tks)
        : primary_tokens(tks), offset(),
          current_scope(new Scope({0, 0})){};
    Preprocess(std::vector<token_t> &tks, Scope::pointer self)
        : primary_tokens(tks), offset() {
        current_scope = self;
    };
    ~Preprocess() = default;

  private:
    // Getter and Setter----------------------------------------------------
    inline TOKEN Current();
    inline const token_t &CurrentToken();
    inline const std::string_view &CurrentTokenName();
    inline TOKEN Peek();
    void Next();
    void reset();

    void Backup(u32);
    // Getter and Setter----------------------------------------------------
  private:
    // for Except, use this when you need Except
    // and you can get a more friendly log, instead of cerr in Except(),
    // always the same line
    i32 result{};
    // someone need custom judge

    // return a code to indicate the result
    i32 Binary(const folly::Function<bool()> &EndJudge);
    i32 Assign(const folly::Function<bool()> &EndJudge);
    i32 Declare(const folly::Function<bool()> &EndJudge);
    i32 Address(const folly::Function<bool()> &EndJudge);
    i32 IfType();
    i32 RetType();

    i32 Paras(Function::pointer fc);
    i32 Callee(Function::pointer callee);
    i32 FuncDecl();

    i32 LoopW(); // while
    i32 LoopF(); // for
    i32 LoopD(); // do

  public:
    i32 Process();

    // Getter and Setter----------------------------------------------------
    const p_token_t &getNext();
    u32 getSize() const;

    const std::vector<p_token_t>& getResult();
    // for test?
    Scope::pointer CurrentScope();
    // Getter and Setter----------------------------------------------------
};

} // namespace vastina

#endif