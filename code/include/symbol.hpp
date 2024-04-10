#ifndef _SYMBOL_H_
#define _SYMBOL_H_

//#include "base/Tree.hpp"
#include "base/vasdef.hpp"
//#include "lexer.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace vastina {
struct token_t {
    TOKEN token;
    std::string_view data;
    unsigned line;

    // some were here just because I am lazy to delete them
    token_t(TOKEN tk);
    token_t(TOKEN tk, const std::string_view &sv);
    token_t(TOKEN tk, std::string_view &&sv);
    token_t(TOKEN tk, const std::string_view &sv, unsigned _line);
    token_t(TOKEN tk, std::string_view &&sv, unsigned _line);

    token_t(const token_t &tk);
    token_t(token_t &&tk);
};
// "create symbol tables for every scope"

class Literal {
  protected:
    bool isTrivial;
};

template <typename ty>
class literal : public Literal { // compile time values like "Hello World",114514,3.14
  private:
    ty value;

  public:
    using Literal::isTrivial;

    literal() = default;
    ~literal() = default;

    inline std::string_view
    Typename() {
        return typeid(ty).name();
    }
};

class Variable {
  protected:
    bool isConstexpr_{};

  public:
    using pointer = Variable *;

    Variable() = default;
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
  private:
    // I need source_location
    // I need reflect or a member to mark the type
  public:
    using Variable::isConst;
    using Variable::isConstexpr_;
    using Variable::Variable;

    variable(){};

    inline std::string_view
    Typename() {
        return typeid(ty).name();
    }
};

class Function {
  protected:
    bool isVoid_{};

  public:
    using pointer = Function *;

    Function() = default;
    ~Function() = default;
    inline pointer
    self() {
        return this;
    }
};

template <typename ty>
class func : public Function {
  public:
    using Function::isVoid_;

    func(){};

    inline std::string_view
    Typename() {
        return typeid(ty).name();
    }

  private:
    ;
};

typedef struct SymbolTable {
    std::unordered_map<std::string_view, Variable> Variables;
    std::unordered_map<std::string_view, Function> functions;

    inline bool
    varExist(const std::string_view &name) // string_view could make
                                           // mistake in some case
    {
        return static_cast<bool>(Variables.count(name));
    }
    inline bool
    funcExist(const std::string_view &name) {
        return static_cast<bool>(functions.count(name));
    }
    // override old value
    inline void
    addVar(const std::string_view &name, Variable &&var) {
        Variables[name] = var;
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
    // override old value
    inline void
    addFunc(const std::string_view &name, Function &&fc) {
        functions[name] = fc;
    }

    // inline Variable::pointer getVar( `the source-location` ) todo
} SymbolTable;

typedef struct range_t {
    unsigned start;
    unsigned end; //[start, end) preprocessed_tokens[start] to
                  // preprocessed_tokens[end-1]

    range_t() : start(0), end(0){};
    inline const range_t &
    operator=(range_t &&other) {
        start = other.start;
        end = other.end;
        return *this;
    }
    range_t(unsigned st, unsigned ed) : start(st), end(ed){};
    range_t(range_t &&other) : start(other.start), end(other.end){};
    range_t(const range_t &other) : start(other.start), end(other.end){};
    inline bool
    isInRange(unsigned index) {
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
    // so bad
    unsigned idchild{};

  public:
    Scope() = delete;
    Scope(range_t &&r) : parent_(nullptr), r_(r), st_(), children_(){};
    Scope(pointer parent, range_t &&r)
        : parent_(parent), r_(r), st_(), children_(){};

    pointer CreateChild(range_t &&);
    pointer getParent();

    void addVar(const std::string_view &name, Variable &&var);
    void addFunc(const std::string_view &name, Function &&fc);
    Variable::pointer getVar(std::string_view name);
    Function::pointer getFunc(std::string_view name);
    bool varExist(const std::string_view &name);
    bool funcExist(const std::string_view &name);

    void setRange(unsigned start, unsigned end);
    const range_t &getRange();

    // for test
    const decltype(children_) &getChildren();
    const SymbolTable &getSymbolTable();

    pointer getNextChild(unsigned);
    Scope::pointer getChildat(unsigned);
};

class Preprocess {
  public:
    enum P_TOKEN { // processed token
        UNKNOW = -1,
        CAL,
        ASSIGN,
        DECL,
        ADDR,
        IF,
        LOOP,
        CALL,
        RET,
    };

    inline static constexpr std::string_view
    p_token_str(P_TOKEN ptk) {
        switch (ptk) {
        case P_TOKEN::CAL:
            return "calculate";
        case P_TOKEN::ASSIGN:
            return "assign";
        case P_TOKEN::DECL:
            return "declare";
        case P_TOKEN::ADDR:
            return "address";
        case P_TOKEN::IF:
            return "if";
        case P_TOKEN::LOOP:
            return "loop";
        case P_TOKEN::CALL:
            return "call";
        case P_TOKEN::RET:
            return "return";
        default:
            return "unknow";
        }
        return {};
    }

    typedef struct p_token_t {
        P_TOKEN tk;
        unsigned start;
        unsigned end;
    } p_token_t;
    //[start, end)
    // see expr.hpp ExpressionUnit

  private:
    std::vector<token_t> &primary_tokens;
    unsigned offset;

    std::vector<p_token_t> results;
    unsigned id = 0;

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
    inline TOKEN Current();
    inline const std::string_view &CurrentTokenName();
    inline TOKEN Peek(unsigned _offset);
    inline TOKEN PreToken(unsigned _offset);
    void Next();
// last==true means if this don't match, stop and return error
#define Except(excepted, last, res) \
    do {                            \
        if (Peek() != excepted) {   \
            if (last) {             \
                EXIT_ERROR          \
            } else {                \
                res = -1;           \
            }                       \
        } else {                    \
            res = 0;                \
        }                           \
    } while (0);
// int Except(TOKEN excepted, bool last);
#define tryNext(excepted, last)                                     \
    do {                                                            \
        Except(excepted, last, result) if (0 == result) { Next(); } \
        else if (last) {                                            \
            RETURN_ERROR                                            \
        }                                                           \
    } while (0)
#define trySkip(excepted, last)                           \
    do {                                                  \
        Except(excepted, last, result) if (0 == result) { \
            Next();                                       \
            Next();                                       \
        }                                                 \
        else if (last) {                                  \
            RETURN_ERROR                                  \
        }                                                 \
    } while (0)

  private:
    // for Except, use this when you need Except
    // and you can get a more friendly log, instead of cerr in Except(),
    // always the same line
    int result{};
    // someone need custom judge

    // please be careful about Cal and Call, so familar
    int CalType(const std::function<bool()> &EndJudge);
    int Assign(const std::function<bool()> &EndJudge);
    int Declare(const std::function<bool()> &EndJudge);
    int Address(const std::function<bool()> &EndJudge);
    int IfType();
    int RetType();

    int Paras(Function::pointer fc);
    int Callee(Function::pointer callee);
    int FuncDecl();

    int LoopW();                                      // while
    int LoopF(const std::function<bool()> &EndJudge); // for
    int LoopD(const std::function<bool()>
                  &EndJudge); // do

    // return a code to indicate the result
  public:
    int Process();
    const p_token_t &getNext();
    unsigned getSize() const;

    // for test?
    Scope::pointer CurrentScope();
};

} // namespace vastina

#endif