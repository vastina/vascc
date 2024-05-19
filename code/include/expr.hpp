#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <vector>

namespace vastina {

class Expression
{
protected:
  u32 level_;

public:
  using pointer = Expression*;
  virtual void Walk() const = 0;
  // for all
  virtual const token_t& getToken() const { throw "donot call this"; }
  // Valexpr
  virtual Value::pointer getValue() const { throw "donot call this"; }

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

public:
  const token_t& getToken() const override;
  void Walk() const override;
};

class ValExpr : public Expression
{
protected:
  Value::pointer value_;

public:
  ValExpr( Value::pointer val );
  ~ValExpr() = default;
  const token_t& getToken() const override { return value_->getSrcloc(); }
  Value::pointer getValue() const override { return value_; }

  void Walk() const override;
};

class BinExpr : public Expression
{
public:
  using pointer = BinExpr*;
  using Node = TreeNode<Expression::pointer>;

protected:
  Node::pointer root_ { nullptr };

public:
  BinExpr() = default;
  BinExpr( Node::pointer root );
  ~BinExpr() { Node::deletenode( root_ ); }

  const token_t& getToken() const override { return root_->data->getToken(); }
  void setRoot( Node::pointer root );
  Node::pointer getRoot() const;

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
  ~CallExpr()
  {
    for ( auto&& para : paras_ )
      delete para;
    paras_.clear();
  }

  const std::vector<BinExpr::pointer>& getParas() { return paras_; }
  void addPara( Node::pointer val );

  void Walk() const override;
};

class AddrExpr : public ValExpr
{};

class nValExpr : public Expression
{};

} // namespace vastina

#endif // _EXPRESSION_H_
