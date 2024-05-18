#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "expr.hpp"
#include "preprocess.hpp"
#include "symbol.hpp"

#include <memory>
#include <vector>

namespace vastina {

using std::make_shared;
using std::shared_ptr;

using TokenPtr = shared_ptr<std::vector<token_t>>;
using ptokens = std::vector<p_token_t>;
using pTokenPtr = shared_ptr<ptokens>;

class Stmt;
using Stmts = std::vector<Stmt*>;

enum class STMTTYPE
{
  UNKNOW = -1,
  Stmt,
  Compound,
  Fdecl,
  Vdecl,
  Binary,
  Cond,
  If,
  Loop,
  Return,
  Call
};

// these classes are just containers

// base statements
class Stmt
{
public:
  using pointer = Stmt*;

protected:
  pointer parent_;

public:
  Stmt( pointer parent ) : parent_( parent ) {}
  pointer getParent() const;

  // for all
  virtual ~Stmt() = default;
  virtual STMTTYPE StmtType() const { return STMTTYPE::Stmt; }

  // CompoundStmt
  virtual void addChildren( Stmt::pointer, i32 = -1 ) {}
  virtual const Stmts& getChildren() const
  {
    constexpr static Stmts s {};
    return s;
  };
  // CondStmt
  virtual void setCondition( Stmt::pointer ) {}
  // VdeclStmt
  virtual void InitWithStmt( Stmt::pointer ) {}
  virtual Stmt::pointer getIniter() const { return nullptr; }
  // BinStmt
  virtual void Parse( const std::vector<token_t>&, range_t ) {}
  // RetStmt
  virtual pointer getResult() const { return nullptr; };
  // FdeclStmt
  // and CallStmt
  virtual Function::pointer getFunc() const { return nullptr; }
  // CallStmt
  // and
  virtual Expression::pointer getData() const { return nullptr; }
  // VdeclStmt
  virtual Variable::pointer getVar() const { return nullptr; }

  // for test
  virtual string_view getName() const { return "Stmt"; };
  virtual void walk() const {}
};

//--------------compound
// statements------------------------------------------------------------------------------------
class CompoundStmt : public Stmt
{
public:
  using pointer = CompoundStmt*;

protected:
  Stmts children_;

public:
  CompoundStmt( Stmt::pointer parent ) : Stmt( parent ), children_ {} {}
  virtual ~CompoundStmt() { children_.clear(); }
  void addChildren( Stmt::pointer, i32 ) override;
  Stmt::pointer getChildat( u32 );
  u32 getStmtSize();
  const Stmts& getChildren() const override;

  STMTTYPE StmtType() const override;
  string_view getName() const override;
};
//--------------compound
// statements------------------------------------------------------------------------------------

//--------------function declare
// statement-----------------------------------------------------------------------------
class FdeclStmt : public CompoundStmt
{
public:
  using pointer = FdeclStmt*;

protected:
  Function::pointer func_;

public:
  FdeclStmt( Stmt::pointer parent, Function::pointer func ) : CompoundStmt( parent ), func_ { func } {}
  Function::pointer getFunc() const override { return func_; }

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------function declare
// statement-----------------------------------------------------------------------------

//--------------binary
// statement---------------------------------------------------------------------------------------
class BinStmt : public Stmt
{
public:
  using pointer = BinStmt*;

protected:
  // Scope::pointer scope_;
  BinExpr::pointer data_;

public:
  BinStmt( Stmt::pointer parent ) : Stmt( parent ), data_( nullptr ) {}
  BinStmt( Stmt::pointer parent, Scope::pointer scope ) : Stmt( parent ), data_( new BinExpr( scope ) ) {}
  ~BinStmt() {}

  void setRoot( BinExpr::Node::pointer );
  BinExpr::pointer getData() const override;

  static Expression::pointer Creator( const token_t&, const Scope::pointer );
  static BinExpr::Node::pointer nodeCreator( const token_t&, Scope::pointer );

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------binary
// statement---------------------------------------------------------------------------------------

//--------------variable declare
// statement-----------------------------------------------------------------------------
class VdeclStmt : public Stmt
{
protected:
  Variable::pointer var_;
  // if not init with val, this is nullptr
  BinStmt::pointer Initer { nullptr };

public:
  VdeclStmt( Stmt::pointer parent, Variable::pointer var ) : Stmt( parent ), var_( var ) {}
  // override but not impl will cause a link error
  void InitWithStmt( Stmt::pointer stmt ) override { Initer = dynamic_cast<BinStmt::pointer>( stmt ); };
  Variable::pointer getVar() const override { return var_; }
  BinStmt::pointer getIniter() const override { return Initer; }

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------variable declare
// statement-----------------------------------------------------------------------------

//--------------condition
// statement------------------------------------------------------------------------------------
class CondStmt : public CompoundStmt
{

protected:
  BinStmt::pointer condition_ { nullptr };

public:
  CondStmt( Stmt::pointer parent ) : CompoundStmt( parent ) {}
  CondStmt( Stmt::pointer parent, BinStmt::pointer cond ) : CompoundStmt( parent ), condition_( cond ) {}
  void setCondition( Stmt::pointer ) override;

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------condition
// statement------------------------------------------------------------------------------------

//--------------loop
// statement-----------------------------------------------------------------------------------------
class LoopStmt : public CondStmt
{

protected:
public:
  LoopStmt( Stmt::pointer parent ) : CondStmt( parent ) {}
  LoopStmt( Stmt::pointer parent, BinStmt::pointer cond ) : CondStmt( parent, cond ) {}

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------loop
// statement-----------------------------------------------------------------------------------------

//--------------if
// statement-------------------------------------------------------------------------------------------
class IfStmt : public CondStmt
{

protected:
public:
  IfStmt( Stmt::pointer parent ) : CondStmt( parent ) {}
  IfStmt( Stmt::pointer parent, BinStmt::pointer cond ) : CondStmt( parent, cond ) {}

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------if
// statement-------------------------------------------------------------------------------------------

//--------------retrun
// statement---------------------------------------------------------------------------------------
class RetStmt : public Stmt
{
protected:
  Stmt::pointer result_;

public:
  RetStmt( Stmt::pointer parent, Stmt::pointer res ) : Stmt( parent ), result_( res ) {}
  Stmt::pointer getResult() const override;

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};
//--------------retrun
// statement---------------------------------------------------------------------------------------

// I think this would mostly work for void
class CallStmt : public Stmt
{
public:
  using pointer = CallStmt*;

protected:
  Expression::pointer callee_;

public:
  CallStmt( Stmt::pointer parent, Expression::pointer callee ) : Stmt( parent ), callee_( callee ) {}
  Function::pointer getFunc() const override { return callee_->getFunc(); }
  Expression::pointer getData() const override { return callee_; }

  STMTTYPE StmtType() const override;
  string_view getName() const override;
  void walk() const override;
};

}; // namespace vastina

#endif