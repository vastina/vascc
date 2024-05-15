#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <memory>
#include <vector>

namespace vastina {

using std::make_shared;
using std::shared_ptr;

class Expression
{
protected:
  u32 level_;

public:
  using pointer = Expression*;
  virtual void Walk() const = 0;
  // ValExpr and OpExpr
  virtual TOKEN getToken() const { return {}; }
  virtual const string_view& getName() const;
  // CallExpr
  virtual Function::pointer getFunc() { return nullptr; }

  virtual ~Expression() = default;

public:
  u32 getLevel();
  void setLevel( u32 );
};

class OpExpr : public Expression
{
protected:
  const token_t& op_;

public:
  using pointer = OpExpr*;
  OpExpr() = delete;
  OpExpr( const token_t& op );
  static pointer Create( TOKEN tk );

public:
  TOKEN getToken() const override;
  const string_view& getName() const override;
  void Walk() const override;
};

class ValExpr : public Expression
{
protected:
  Value::pointer value_;

public:
  ValExpr( Value::pointer val );
  TOKEN getToken() const override;
  const string_view& getName() const override;
  void Walk() const override;
};

class BinExpr : public Expression
{
public:
  using pointer = BinExpr*;
  using Node = TreeNode<Expression::pointer>;

protected:
  Node::pointer root_;
  Scope::pointer scope_;

public:
  BinExpr( Node::pointer root );
  BinExpr( Scope::pointer scope );
  BinExpr( Node::pointer root, Scope::pointer scope );

  void setRoot( Node::pointer root );
  Node::pointer getRoot() const;
  Scope::pointer getScope() const;

  void Walk() const override;
};

class CallExpr : public ValExpr
{
public:
  using pointer = CallExpr*;
  using Node = TreeNode<Expression::pointer>;

protected:
  std::vector<BinExpr::pointer> paras_;

public:
  CallExpr( Value::pointer val );
  Function::pointer getFunc() override;
  void addPara( Node::pointer val );
  void Walk() const override;
};

class AddrExpr : public ValExpr
{};

class nValExpr : public Expression
{};

typedef struct BracketCount
{
  u32 open = 0;
  u32 close = 0;
} BracketCount;

} // namespace vastina

#endif // _EXPRESSION_H_
