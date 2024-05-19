#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "expr.hpp"
#include "preprocess.hpp"

#include <vector>

namespace vastina {

using ptokens = std::vector<p_token_t>;

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
  GOTO,
  BC,
  If,
  Loop,
  FOR,  //ok, use this for for
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

  inline static void deleteStmt( Stmt::pointer stmt )
  {
    for ( auto&& child : stmt->getChildren() )
      deleteStmt( child );
    delete stmt;
  }

  // for all
  virtual ~Stmt() = default;
  virtual STMTTYPE StmtType() const { throw "donot call this"; }

  // CompoundStmt
  virtual void addChildren( Stmt::pointer, i32 = -1 ) { throw "donot call this"; }
  virtual const Stmts& getChildren() const
  {
    constexpr static Stmts s {};
    return s;
  };

  virtual Stmt::pointer getStmt() const { throw "donot call this"; }
  virtual void setStmt( Stmt::pointer ) { throw "donot call this"; }

  virtual Expression::pointer getExpr() const { throw "donot call this"; }

  virtual Function::pointer getFunc() const { throw "donot call this"; }
  virtual Variable::pointer getVar() const { throw "donot call this"; }

  virtual bool getState() const { throw "donot call this"; }

  // for test
  virtual string_view for_test_getName() const { throw "donot call this"; };
  virtual void walk() const { throw "donot call this"; }
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
  const Stmts& getChildren() const override;

  virtual STMTTYPE StmtType() const override;
  virtual string_view for_test_getName() const override;
  virtual void walk() const override;
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
  string_view for_test_getName() const override;
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
  BinExpr::pointer data_;

public:
  BinStmt( Stmt::pointer parent ) : Stmt( parent ), data_( new BinExpr() ) {}
  ~BinStmt() { delete data_; }

  void setRoot( BinExpr::Node::pointer );
  BinExpr::pointer getExpr() const override;

  static Expression::pointer Creator( const token_t&, const Scope::pointer );
  static BinExpr::Node::pointer nodeCreator( const token_t&, Scope::pointer );

  STMTTYPE StmtType() const override;
  string_view for_test_getName() const override;
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
  void setStmt( Stmt::pointer stmt ) override { Initer = dynamic_cast<BinStmt::pointer>( stmt ); };
  Variable::pointer getVar() const override { return var_; }
  BinStmt::pointer getStmt() const override { return Initer; }

  STMTTYPE StmtType() const override;
  string_view for_test_getName() const override;
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
  void setStmt( Stmt::pointer ) override;
  Stmt::pointer getStmt() const override { return condition_; }

  virtual STMTTYPE StmtType() const override;
  virtual string_view for_test_getName() const override;
  virtual void walk() const override;
};
//--------------condition
// statement------------------------------------------------------------------------------------

//--------------loop
// statement-----------------------------------------------------------------------------------------
class LoopStmt : public CondStmt
{

protected:
  bool with_do_ {};
public:
  LoopStmt( Stmt::pointer parent, BinStmt::pointer cond, bool with_do ) : CondStmt( parent, cond ), with_do_(with_do) {}
  bool getState() const override { return with_do_; }

  STMTTYPE StmtType() const override;
  string_view for_test_getName() const override;
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

  STMTTYPE StmtType() const override final;
  string_view for_test_getName() const override;
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
  Stmt::pointer getStmt() const override;

  STMTTYPE StmtType() const override;
  string_view for_test_getName() const override;
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
  Expression::pointer getExpr() const override { return callee_; }

  STMTTYPE StmtType() const override;
  string_view for_test_getName() const override;
  void walk() const override;
};

// this is not goto, for switch
class GotoStmt : public CompoundStmt {
public:
  using pointer = GotoStmt*;

protected:
  BinStmt::pointer entry;

public:

};

// for break and continue and ...
class BcStmt : public Stmt {
public:
  using pointer = BcStmt*;
protected:
  OpExpr::pointer data_;

public:
  BcStmt( Stmt::pointer parent, OpExpr::pointer op ) : Stmt( parent ), data_( op ) {}
  ~BcStmt(){
    delete data_;
  }
  Expression::pointer getExpr() const override { return data_; }

  inline STMTTYPE StmtType() const override { return STMTTYPE::BC; };
  inline string_view for_test_getName() const override { return data_->getToken().name; } ;
  inline void walk() const override {
    data_->Walk();
  };
};

}; // namespace vastina

#endif