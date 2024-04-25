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
    // CallExpr
    virtual Function::pointer getFunc() { return nullptr; };
    virtual void Parse(const std::vector<token_t> &, u32, u32 &) {};

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

    inline void Walk() const override {print("opexpr, data: {}\n", op_.name);}
};

class ValExpr : public Expression {
  protected:
    Value::pointer value_;
    const token_t &val_;

  public:
    ValExpr(Value::pointer val, const token_t &tk) : value_(val), val_(tk){};
    inline TOKEN getToken() override { return val_.token; };
    inline std::string_view getName() override { return val_.name; };

    inline void Walk() const override {print("valexpr, data: {}\n", val_.name);}
};

class BinExpr : public Expression {
  public:
    using pointer = BinExpr *;
    using Node = TreeNode<Expression::pointer>;
  protected:
    Node::pointer root_;
    Scope::pointer scope_;
  
  public:
    BinExpr(Node::pointer root) : root_{root} {};
    BinExpr(Scope::pointer scope) : root_{nullptr}, scope_{scope} {};
    BinExpr(Node::pointer root ,Scope::pointer scope) : root_{root}, scope_{scope} {};

    inline void setRoot(Node::pointer root) { root_ = root; }

    //static Expression::pointer Creator(const token_t &, const Scope::pointer);
    //static Node::pointer nodeCreator(const token_t &, Scope::pointer);
    inline void Walk() const override {root_->Walk(PREORDER, 
      [](const Expression::pointer &_data) { _data->Walk();}) ; }
};

class CallExpr : public ValExpr {
  public:
    using pointer = CallExpr *;

  protected:
    std::vector<BinExpr::pointer> paras_;
    //    Function::pointer func;
  public:
    CallExpr(Value::pointer val, const token_t &tk) : ValExpr(val, tk), paras_{} {};
    void Parse(const std::vector<token_t> &, u32, u32 &) override {};
    inline Function::pointer getFunc() override { return dynamic_cast<Function::pointer>(value_); };
    inline void addPara(typename TreeNode<Expression::pointer>::pointer val) { paras_.push_back(new BinExpr(val)); };

    inline std::string_view getName() override { return val_.name; };
    inline void Walk() const override {
      print("call expr, walk params\n");
      for(auto&& para: paras_) para->Walk();
    }
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