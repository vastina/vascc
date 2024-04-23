#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

namespace vastina {

using std::make_shared;
using std::shared_ptr;

// some classes here should be deleted

template <typename ty>
concept NotVoid = !std::is_same_v<ty, void>;

class Expression {
  protected:
    u32 level_;

  public:
    using pointer = Expression *;
    virtual void Walk() const {};
    // ValExpr and OpExpr
    virtual TOKEN getToken() { return {}; }
    virtual std::string_view getName() { return {}; };
    //CallExpr
    virtual Function::pointer getFunc() {return nullptr;};
    virtual void Parse(const std::vector<token_t>&, u32, u32&) {};

    virtual ~Expression() = default;

  public:
    inline u32 getLevel() { return level_; };
    inline void setLevel(u32 level) { level_ = level; };
};

class OpExpr : public Expression {
  protected:
    const token_t &op_;

  public:
    using pointer = OpExpr *;
    OpExpr() = delete;
    OpExpr(const token_t &op) : op_(op){};
    static inline pointer Create(TOKEN tk) {
        return new OpExpr(tk);
    }

  public:
    inline TOKEN getToken() override { return op_.token; };
    inline std::string_view getName() override { return op_.name; };
};

class ValExpr : public Expression {
  protected:
    Value::pointer value_;
    const token_t& val_;

  public:
    ValExpr(Value::pointer val, const token_t &tk) : value_(val), val_(tk){};
    inline TOKEN getToken() override { return val_.token; };
    inline std::string_view getName() override { return val_.name; };
};

class CallExpr : public ValExpr {
  public:
    using pointer = CallExpr *;
  protected:
    std::vector<typename TreeNode<Expression::pointer>::pointer> paras_;
//    Function::pointer func;
  public:
    CallExpr(Value::pointer val, const token_t &tk) : ValExpr(val, tk), paras_{}{};
    void Parse(const std::vector<token_t>&, u32, u32&) override{};
    inline Function::pointer getFunc() override{return dynamic_cast<Function::pointer>(value_);};
    inline void addPara(typename TreeNode<Expression::pointer>::pointer val) {paras_.push_back(val);};

    inline std::string_view getName() override { return val_.name; };
};

class AddrExpr : public ValExpr {
};

// 不能被作为值赋值给别的lval 与 不求值表达式 不是一回事, 这里为了方便取的是前者
class nValExpr : public Expression {
};

typedef struct BracketCount {
    u32 open = 0;
    u32 close = 0;
} BracketCount;

} // namespace vastina


#endif