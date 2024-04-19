#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "base/Tree.hpp"
#include "expr.hpp"
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
using Stmts = std::vector<Stmt *>;

class Stmt {
  public:
    using pointer = Stmt *;

  protected:
    range_t r_;
    Scope::pointer scope_; // the scope it belongs to?

  public:
    ~Stmt() = default;
};

class CompoundStmt : public Stmt {
  public:
    using pointer = CompoundStmt *;

  protected:
    Stmts children_;
};

class FdeclStmt : public Stmt {
  public:
    using pointer = FdeclStmt*;
  protected:
    Function::pointer func_;
    CompoundStmt::pointer body_;

  public:
    FdeclStmt(Function::pointer func) : func_(func), body_() {};
    i32 Parse(const std::vector<token_t>&, const std::vector<p_token_t>&, range_t);
};

class VdeclStmt : public Stmt {
  protected:
    Variable::pointer var_;
};

class BinStmt : public Stmt {
  public:
    using pointer = BinStmt *;

  protected:
    TreeNode<Expression> root_;

  public:
};

class LoopStmt : public Stmt {

  protected:
    BinStmt::pointer condition_;
    CompoundStmt::pointer body_;
};

class IfStmt : public Stmt {

  protected:
    BinStmt::pointer condition_;
    CompoundStmt::pointer body_;
};

class RetStmt : public Stmt{
  protected:
    Expression::pointer result_;
};

}; // namespace vastina

#endif