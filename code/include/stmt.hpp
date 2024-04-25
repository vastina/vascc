#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "expr.hpp"
#include "symbol.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace vastina {

using std::make_shared;
using std::shared_ptr;

using TokenPtr = shared_ptr<std::vector<token_t>>;
using ptokens = std::vector<p_token_t>;
using pTokenPtr = shared_ptr<ptokens>;

class Stmt;
using Stmts = std::vector<Stmt *>;

// base statements
class Stmt {
  public:
    using pointer = Stmt *;

  protected:
    pointer parent_;

  public:
    Stmt(pointer parent) : parent_(parent){};
    virtual ~Stmt() = default;
    inline pointer getParent() const { return parent_; }
    // CompoundStmt
    virtual void addChildren(Stmt::pointer, i32 = -1){};
    virtual const Stmts &getChildren() const {
        constexpr static Stmts s{};
        return s;
    };
    // CondStmt
    virtual void setCondition(Stmt::pointer){};
    // VdeclStmt
    virtual void InitWithStmt(Stmt::pointer){};
    // BinStmt
    virtual void Parse(const std::vector<token_t> &, range_t) {};
    // RetStmt
    virtual pointer getResult() const { return {}; };

    // for test
    virtual std::string_view getName() const { return "Stmt"; };
    virtual void walk() const {};
};

//--------------compound statements------------------------------------------------------------------------------------
class CompoundStmt : public Stmt {
  public:
    using pointer = CompoundStmt *;

  protected:
    Stmts children_;

  public:
    CompoundStmt(Stmt::pointer parent) : Stmt(parent), children_{} {};
    virtual ~CompoundStmt() { children_.clear(); }
    void addChildren(Stmt::pointer, i32) override;
    inline Stmt::pointer getChildat(u32 pos) { return children_.at(pos); }
    inline u32 getStmtSize() { return children_.size(); }
    inline const Stmts &getChildren() const override { return children_; }

    inline std::string_view getName() const override { return "CompoundStmt"; };
};
//--------------compound statements------------------------------------------------------------------------------------

//--------------function declare statement-----------------------------------------------------------------------------
class FdeclStmt : public CompoundStmt {
  public:
    using pointer = FdeclStmt *;

  protected:
    Function::pointer func_;

  public:
    FdeclStmt(Stmt::pointer parent, Function::pointer func) : CompoundStmt(parent), func_{func} {};

    inline std::string_view getName() const override { return "FdeclStmt"; };
    inline void walk() const override{ print("function name: {}\n", func_->getName()); };
};
//--------------function declare statement-----------------------------------------------------------------------------

//--------------binary statement---------------------------------------------------------------------------------------
class BinStmt : public Stmt {
    // make it BinExpr maybe better
  public:
    using pointer = BinStmt *;

  protected:
    // typename TreeNode<Expression::pointer>::pointer root_;
    // Scope::pointer scope_;
    BinExpr::pointer data_;

  public:
    BinStmt(Stmt::pointer parent) : Stmt(parent), data_(nullptr) {};
    BinStmt(Stmt::pointer parent, Scope::pointer scope) : Stmt(parent), data_(new BinExpr(scope)) {};
    // typename TreeNode<Expression::pointer>::pointer
    // doParse(const std::vector<token_t> &primary_tokens, u32 end, u32 &offset);
    // void Parse(const std::vector<token_t> &, range_t) override;
    inline void setRoot(typename TreeNode<Expression::pointer>::pointer root) { data_->setRoot(root); }

    static Expression::pointer Creator(const token_t &, const Scope::pointer);
    static BinExpr::Node::pointer nodeCreator(const token_t &, Scope::pointer);

    inline std::string_view getName() const override { return "BinStmt"; };
    inline void walk() const override {print("binary walk\n"); data_->Walk();}
};
//--------------binary statement---------------------------------------------------------------------------------------

//--------------variable declare statement-----------------------------------------------------------------------------
class VdeclStmt : public Stmt {
  protected:
    Variable::pointer var_;
    // if not init with val, this is nullptr
    BinStmt::pointer Initer{nullptr};

  public:
    VdeclStmt(Stmt::pointer parent, Variable::pointer var) : Stmt(parent), var_(var){};
    // override but not impl will cause a link error
    void InitWithStmt(Stmt::pointer) override{};

    inline std::string_view getName() const override { return "VdeclStmt"; };
    inline void walk() const override {print("Vdecl, var-name: {}\n", var_->getName()); }
};
//--------------variable declare statement-----------------------------------------------------------------------------

//--------------condition statement------------------------------------------------------------------------------------
class CondStmt : public CompoundStmt {

  protected:
    BinStmt::pointer condition_{nullptr};

  public:
    CondStmt(Stmt::pointer parent) : CompoundStmt(parent){};
    CondStmt(Stmt::pointer parent, BinStmt::pointer cond) : CompoundStmt(parent), condition_(cond){};
    void setCondition(Stmt::pointer) override;

    inline std::string_view getName() const override { return "CondStmt"; };
    inline void walk() const override {print("CondStmt, walk condition\n"); condition_->walk(); }
};
//--------------condition statement------------------------------------------------------------------------------------

//--------------loop statement-----------------------------------------------------------------------------------------
class LoopStmt : public CondStmt {

  protected:
  public:
    LoopStmt(Stmt::pointer parent) : CondStmt(parent){};
    LoopStmt(Stmt::pointer parent, BinStmt::pointer cond) : CondStmt(parent, cond){};

    inline std::string_view getName() const override { return "LoopStmt"; };
    inline void walk() const override {print("LoopStmt, walk condition\n"); condition_->walk(); }
};
//--------------loop statement-----------------------------------------------------------------------------------------

//--------------if statement-------------------------------------------------------------------------------------------
class IfStmt : public CondStmt {

  protected:
  public:
    IfStmt(Stmt::pointer parent) : CondStmt(parent){};
    IfStmt(Stmt::pointer parent, BinStmt::pointer cond) : CondStmt(parent, cond){};

    inline std::string_view getName() const override { return "IfStmt"; };
    inline void walk() const override {print("IfStmt, walk condition\n"); condition_->walk(); }
};
//--------------if statement-------------------------------------------------------------------------------------------

//--------------retrun statement---------------------------------------------------------------------------------------
class RetStmt : public Stmt {
  protected:
    Stmt::pointer result_;

  public:
    RetStmt(Stmt::pointer parent, Stmt::pointer res) : Stmt(parent), result_(res){};
    inline pointer getResult() const override { return result_; };

    inline std::string_view getName() const override { return "RetStmt"; };
    inline void walk() const override {print("RetStmt, walk result\n"); result_->walk(); }
};
//--------------retrun statement---------------------------------------------------------------------------------------

// I think this would mostly work for void
class CallStmt : public Stmt {
  public:
    using pointer = CallStmt *;

  protected:
    Expression::pointer callee_;

  public:
    CallStmt(Stmt::pointer parent, Expression::pointer callee) : Stmt(parent), callee_(callee){};

    inline std::string_view getName() const override { return "CallStmt"; };
    inline void walk() const override {print("CallStmt, walk callee\n"); callee_->Walk(); }
};

}; // namespace vastina

#endif