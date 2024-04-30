#include "stmt.hpp"
#include "base/Tree.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "expr.hpp"
#include "symbol.hpp"

namespace vastina {

void CompoundStmt::addChildren( Stmt::pointer child, i32 pos = -1 )
{
  if ( 0 > pos )
    children_.push_back( child );
  else
    children_.insert( children_.begin() + pos, child );
}

void CondStmt::setCondition( Stmt::pointer condtion )
{
  condition_ = dynamic_cast<BinStmt::pointer>( condtion );
}

#define REPORT_MSG( msg )                                                                                          \
  {                                                                                                                \
    LEAVE_MSG( msg );                                                                                              \
    print( "token name: {} at line: {}\n", token.name, token.line );                                               \
    RETURN_NULL;                                                                                                   \
  }
Expression::pointer BinStmt::Creator( const token_t& token, const Scope::pointer scope )
{
  auto tk = token.token;
  switch ( token_type( tk ) ) {
    case TOKEN_TYPE::TYPE:
      THIS_NOT_SUPPORT( "type cast" );
    case TOKEN_TYPE::BRAC:
    case TOKEN_TYPE::OPERATOR:
      return new OpExpr( token );
    case TOKEN_TYPE::VALUE:
      if ( TOKEN::SYMBOL == tk ) {
        if ( !scope->varExist( token.name ) ) {
          REPORT_MSG( "var not exist" );
        } else {
          return new ValExpr( scope->getVar( token.name ) );
        }
      } else if ( TOKEN::SYMBOLF == tk ) {
        if ( !scope->funcExist( token.name ) ) {
          REPORT_MSG( "func not exist" );
        } else {
          return new ValExpr( scope->getFunc( token.name ) );
        }
      } else {
        return new ValExpr( new literal( token ) );
      }

      break;
    default:
      REPORT_MSG( "no match token_type" );
  }
}

typename TreeNode<Expression::pointer>::pointer BinStmt::nodeCreator( const token_t& tk, Scope::pointer scope )
{
  auto data = Creator( tk, scope );
  data->setLevel( Level( tk.token ) );
  return new TreeNode<Expression::pointer>( data );
}

// do not modify this even you know what you are doing: commit name is parser_2 and the following one, hash is
// bd1c425 and b6ba847

} // namespace vastina
