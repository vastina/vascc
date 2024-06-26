#include "stmt.hpp"
#include "base/log.hpp"

namespace vastina {

Stmt::pointer Stmt::getParent() const
{
  return parent_;
}

const Stmts& CompoundStmt::getChildren() const
{
  return children_;
}
STMTTYPE CompoundStmt::StmtType() const
{
  return STMTTYPE::Compound;
}
string_view CompoundStmt::for_test_getName() const
{
  return "CompoundStmt";
}
void CompoundStmt::walk() const
{
  print( "Compound\n" );
}

void CompoundStmt::addChildren( Stmt::pointer child, i32 pos = -1 )
{
  if ( 0 > pos )
    children_.push_back( child );
  else
    children_.insert( children_.begin() + pos, child );
}

void CondStmt::setStmt( Stmt::pointer condtion )
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
          print( "{}, {}\n", scope->getRange().start, scope->getRange().end );
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
        return new ValExpr( new literal( token, token.token ) );
      }

      break;
    default:
      REPORT_MSG( "no match token_type" );
  }
}

BinExpr::Node::pointer BinStmt::nodeCreator( const token_t& tk, Scope::pointer scope )
{
  auto data = Creator( tk, scope );
  data->setLevel( Level( tk.token ) );
  return new BinExpr::Node( data );
}

// do not modify this even you know what you are doing: commit name is parser_2 and the following one, hash is
// bd1c425 and b6ba847

STMTTYPE FdeclStmt::StmtType() const
{
  return STMTTYPE::Fdecl;
}
string_view FdeclStmt::for_test_getName() const
{
  return "FdeclStmt";
}
void FdeclStmt::walk() const
{
  print( "function name: {}\n", func_->getSrcloc().name );
}

void BinStmt::setRoot( BinExpr::Node::pointer root )
{
  data_->setRoot( root );
}
BinExpr::pointer BinStmt::getExpr() const
{
  return data_;
}

STMTTYPE BinStmt::StmtType() const
{
  return STMTTYPE::Binary;
}
string_view BinStmt::for_test_getName() const
{
  return "BinStmt";
}
void BinStmt::walk() const
{
  print( "binary walk\n" );
  data_->Walk();
}

STMTTYPE VdeclStmt::StmtType() const
{
  return STMTTYPE::Vdecl;
}
string_view VdeclStmt::for_test_getName() const
{
  return "VdeclStmt";
}
void VdeclStmt::walk() const
{
  print( "Vdecl, var-name: {}\n", var_->getSrcloc().name );
  return nullptr == Initer ? print( "not init with val\n" ) : ( [&] {
    print( "init with val, walk initer\n" );
    Initer->walk();
  } )();
}

STMTTYPE CondStmt::StmtType() const
{
  return STMTTYPE::Cond;
}
string_view CondStmt::for_test_getName() const
{
  return "CondStmt";
}
void CondStmt::walk() const
{
  print( "CondStmt, walk condition\n" );
  condition_->walk();
}

STMTTYPE LoopStmt::StmtType() const
{
  return STMTTYPE::Loop;
}
string_view LoopStmt::for_test_getName() const
{
  return "LoopStmt";
}
void LoopStmt::walk() const
{
  print( "LoopStmt, walk condition\n" );
  condition_->walk();
}

STMTTYPE IfStmt::StmtType() const
{
  return STMTTYPE::If;
}
string_view IfStmt::for_test_getName() const
{
  return "IfStmt";
}
void IfStmt::walk() const
{
  print( "IfStmt, walk condition\n" );
  condition_->walk();
}

Stmt::pointer RetStmt::getStmt() const
{
  return result_;
}

STMTTYPE RetStmt::StmtType() const
{
  return STMTTYPE::Return;
}
string_view RetStmt::for_test_getName() const
{
  return "RetStmt";
}
void RetStmt::walk() const
{
  print( "RetStmt, walk result\n" );
  result_->walk();
}

STMTTYPE CallStmt::StmtType() const
{
  return STMTTYPE::Call;
}
string_view CallStmt::for_test_getName() const
{
  return "CallStmt";
}
void CallStmt::walk() const
{
  print( "CallStmt, walk callee\n" );
  callee_->Walk();
}

} // namespace vastina
