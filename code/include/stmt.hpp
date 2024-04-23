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

//base statements
class Stmt {
  public:
    using pointer = Stmt *;

  protected:
    pointer parent_;

  public:
    Stmt(pointer parent) : parent_(parent){};
    virtual ~Stmt() = default;
    inline pointer getParent(){return parent_;}
//CompoundStmt
    virtual void addChildren(Stmt::pointer, i32=-1) {};
//CondStmt
    virtual void setCondition(Stmt::pointer){};
//VdeclStmt
    virtual void InitWithStmt(Stmt::pointer){};
//BinStmt
    virtual void Parse(const std::vector<token_t> &, range_t) {};

// for test
    virtual std::string_view getName(){return "Stmt";};
};

//compound statements
class CompoundStmt : public Stmt {
  public:
    using pointer = CompoundStmt *;

  protected:
    Stmts children_;
  
  public:
    CompoundStmt(Stmt::pointer parent) : Stmt(parent), children_{}{};
    virtual ~CompoundStmt(){children_.clear();}
    void addChildren(Stmt::pointer, i32) override ;

    inline Stmt::pointer getChildat(u32 pos){return children_.at(pos);}
    inline u32 getStmtSize(){return children_.size();}
    inline std::string_view getName() override {return "CompoundStmt";} ;
};

//function declare
class FdeclStmt : public CompoundStmt {
  public:
    using pointer = FdeclStmt*;
  protected:
    Function::pointer func_;

  public:
    FdeclStmt(Function::pointer func, Stmt::pointer parent) : CompoundStmt(parent) ,func_(func) {};

    inline std::string_view getName() override {return "FdeclStmt";} ;
};

//binary statement
class BinStmt : public Stmt {
  public:
    using pointer = BinStmt *;

  protected:
    typename TreeNode<Expression::pointer>::pointer root_;
    Scope::pointer scope_;

  public:
    BinStmt(Stmt::pointer parent) : Stmt(parent), root_{nullptr}, scope_{nullptr} {};
    BinStmt(Stmt::pointer parent, Scope::pointer scope) : Stmt(parent), root_{nullptr}, scope_{scope} {};
    typename  TreeNode<Expression::pointer>::pointer 
      doParse(const std::vector<token_t> &primary_tokens, u32 end, u32 &offset);
    void Parse(const std::vector<token_t> &, range_t) override;
    static Expression::pointer Creator(const token_t&, const Scope::pointer);
    typename  TreeNode<Expression::pointer>::pointer
      nodeCreator(const token_t& tk);

    inline std::string_view getName() override {return "BinStmt";} ;
};

//variable declare
class VdeclStmt : public Stmt {
  protected:
    Variable::pointer var_;
// if not init with val, this is nullptr
    BinStmt::pointer Initer{nullptr};
  
  public:
    VdeclStmt(Stmt::pointer parent, Variable::pointer var) : Stmt(parent), var_(var) {};
//override but not impl will cause a link error
    void InitWithStmt(Stmt::pointer) override{};

    inline std::string_view getName() override {return "VdeclStmt";} ;
};

//condition statement
class CondStmt : public CompoundStmt {

  protected:
    BinStmt::pointer condition_;
  public:
    CondStmt(Stmt::pointer parent) : CompoundStmt(parent){};
    void setCondition(Stmt::pointer) override;

    inline std::string_view getName() override {return "CondStmt";} ;
};

//loop statement
class LoopStmt : public CondStmt {

  protected:
    BinStmt::pointer condition_;

  public:
    LoopStmt(Stmt::pointer parent) : CondStmt(parent){};

    inline std::string_view getName() override {return "LoopStmt";} ;
};

//if statement
class IfStmt : public CondStmt {

  protected:
    BinStmt::pointer condition_;

  public:
    IfStmt(Stmt::pointer parent) : CondStmt(parent){};

    inline std::string_view getName() override {return "IfStmt";} ;
};

//retrun statement
class RetStmt : public Stmt{
  protected:
    Expression::pointer result_;

  public:
    inline std::string_view getName() override {return "RetStmt";} ;
};

}; // namespace vastina

#endif