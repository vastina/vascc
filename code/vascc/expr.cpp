#include "expr.hpp"

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

// OpExpr
OpExpr::OpExpr( const token_t& op ) : op_( op ) {}
const token_t& OpExpr::getToken() const
{
  return op_;
}
void OpExpr::Walk() const
{
  std::cout << "opexpr, data: " << op_.name << "\n";
}

// ValExpr
ValExpr::ValExpr( Value::pointer val ) : value_( val ) {}
void ValExpr::Walk() const
{
  std::cout << "valexpr, data: " << value_->getSrcloc().name << "\n";
}

// BinExpr
BinExpr::BinExpr( Node::pointer root ) : root_( root ) {}

void BinExpr::setRoot( Node::pointer root )
{
  root_ = root;
}
BinExpr::Node::pointer BinExpr::getRoot() const
{
  return root_;
}

void BinExpr::Walk() const
{
  root_->Walk( walk_order::PREORDER, []( const Expression::pointer& _data ) { _data->Walk(); } );
}

// CallExpr
CallExpr::CallExpr( Value::pointer val ) : ValExpr( val ), paras_() {}
void CallExpr::addPara( CallExpr::Node::pointer val )
{
  paras_.push_back( new BinExpr( val ) );
}

void CallExpr::Walk() const
{
  std::cout << "call expr, callee name: " << value_->getSrcloc().name
            << ( paras_.empty() ? ", no params\n" : ", walk params\n" );
  for ( auto&& para : paras_ ) {
    para->Walk();
  }
}

} // namespace vastina
