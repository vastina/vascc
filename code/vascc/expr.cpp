#include "expr.hpp"
#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"
#include <iostream>
#include <vector>

namespace vastina {

// Expression
u32 Expression::getLevel()
{
  return level_;
}
void Expression::setLevel( u32 level )
{
  level_ = level;
}
const string_view& Expression::getName() const
{
  const_str_t sss {};
  return sss;
}

// OpExpr
OpExpr::OpExpr( const token_t& op ) : op_( op ) {}
OpExpr::pointer OpExpr::Create( TOKEN tk )
{
  return new OpExpr( tk );
}
TOKEN OpExpr::getToken() const
{
  return op_.token;
}
const string_view& OpExpr::getName() const
{
  return op_.name;
}
void OpExpr::Walk() const
{
  std::cout << "opexpr, data: " << op_.name << "\n";
}

// ValExpr
ValExpr::ValExpr( Value::pointer val ) : value_( val ) {}
TOKEN ValExpr::getToken() const
{
  return value_->getSrcloc().token;
}
const string_view& ValExpr::getName() const
{
  return value_->getName();
}
void ValExpr::Walk() const
{
  std::cout << "valexpr, data: " << value_->getName() << "\n";
}

// BinExpr
BinExpr::BinExpr( Node::pointer root ) : root_( root ) {}
BinExpr::BinExpr( Scope::pointer scope ) : root_( nullptr ), scope_( scope ) {}
BinExpr::BinExpr( Node::pointer root, Scope::pointer scope ) : root_( root ), scope_( scope ) {}

void BinExpr::setRoot( Node::pointer root )
{
  root_ = root;
}
BinExpr::Node::pointer BinExpr::getRoot() const
{
  return root_;
}
Scope::pointer BinExpr::getScope() const
{
  return scope_;
}

TOKEN BinExpr::getToken() const {
  return root_->data->getToken();
}

void BinExpr::Walk() const
{
  root_->Walk( walk_order::PREORDER, []( const Expression::pointer& _data ) { _data->Walk(); } );
}

// CallExpr
CallExpr::CallExpr( Value::pointer val ) : ValExpr( val ), paras_() {}
Function::pointer CallExpr::getFunc()
{
  return dynamic_cast<Function::pointer>( value_ );
}
void CallExpr::addPara( CallExpr::Node::pointer val )
{
  paras_.push_back( new BinExpr( val ) );
}

void CallExpr::Walk() const
{
  std::cout << "call expr, callee name: " << value_->getName()
            << ( paras_.empty() ? ", no params\n" : ", walk params\n" );
  for ( auto&& para : paras_ ) {
    para->Walk();
  }
}

} // namespace vastina
