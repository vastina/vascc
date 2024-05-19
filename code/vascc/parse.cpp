#include "parse.hpp"

#include "base/Tree.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "stmt.hpp"
#include "symbol.hpp"

#include <queue>

namespace vastina {

inline P_TOKEN Parser::Current()
{
  return processed_tokens_.at( p_offset_ ).tk;
}

inline const p_token_t& Parser::CurrentToken()
{
  return processed_tokens_.at( p_offset_ );
}

inline P_TOKEN Parser::Peek()
{
  return processed_tokens_.at( p_offset_ + 1 ).tk;
}

inline const p_token_t& Parser::PeekToken()
{
  return processed_tokens_.at( p_offset_ + 1 );
}

inline const p_token_t& Parser::Peekat( u32 offset )
{
  return processed_tokens_.at( offset );
}

inline void Parser::Next()
{
  p_offset_++;
}

inline const token_t& Parser::PeekPrtat( u32 offst )
{
  return primary_tokens_.at( offst );
}

i32 Parser::Parse()
{
  const u32 processed_tokens_size = processed_tokens_.size();
  while ( p_offset_ < processed_tokens_size ) {
    switch ( Current() ) {
      case P_TOKEN::BINARY: {
        EXCEPT_ZERO( Binary );
        break;
      }
      case P_TOKEN::FDECL: {
        EXCEPT_ZERO( Fdecl );
        break;
      }
      case P_TOKEN::VDECL: {
        EXCEPT_ZERO( Vdecl );
        break;
      }
      case P_TOKEN::ADDR: {

        // when will this be done................................................................
        break;
      }
      case P_TOKEN::IF: {
        EXCEPT_ZERO( Ifer );
        break;
      }
      case P_TOKEN::LOOP: {
        EXCEPT_ZERO( Loop );
        break;
      }
      case P_TOKEN::CALL: {
        EXCEPT_ZERO( Call );
        break;
      }
      case P_TOKEN::RET: {
        EXCEPT_ZERO( Ret );
        break;
      }
      case P_TOKEN::END: {
        break;
      }
      default:
        EXIT_ERROR;
    }

    Next();
    // print("\np_offset: {}, end{}\n", p_offset_, scope_->getRange().end);
    if ( p_offset_ >= scope_->getRange().end ) {
      // should avoid fall nullptr here?
      if ( scope_->getParent() != nullptr )
        scope_ = scope_->getParent();
      if ( current_stmt_->getParent() != nullptr )
        current_stmt_ = current_stmt_->getParent();
    }
    if ( scope_->idchild_ < scope_->getChildren().size() )
      if ( p_offset_ >= scope_->getChildat( scope_->idchild_ )->getRange().start ) {
        scope_ = scope_->getNextChild();
        auto newCompound { new CompoundStmt( current_stmt_ ) };
        current_stmt_->addChildren( newCompound );
        current_stmt_ = newCompound;
      }
  }

  while ( scope_->getParent() != nullptr )
    scope_ = scope_->getParent();
  scope_->reSet();

  return 0;
}

i32 Parser::Vdecl()
{
  auto var = scope_->getVar( PeekPrtat( CurrentToken().start ).name );
  auto vstmt = new VdeclStmt( current_stmt_, var );

  current_stmt_->addChildren( vstmt );

  if ( Peek() == P_TOKEN::BINARY ) { // if with sth to init
    Next();
    vstmt->setStmt( Binary( { CurrentToken().start, CurrentToken().end } ) );
  }

  return 0;
}

i32 Parser::Fdecl()
{
  // todo, if there's only declare, no body
  // and this is so stupid
  auto func = scope_->getFunc( PeekPrtat( CurrentToken().start + 1 ).name );
  if ( nullptr == func )
    RETURN_ERROR
  auto fstmt = new FdeclStmt( current_stmt_, func );
  current_stmt_->addChildren( fstmt );
  current_stmt_ = fstmt;
  scope_ = scope_->getNextChild();

  return 0;
}

i32 Parser::Call()
{
  auto callee = Callee( p_offset_ );
  auto callstmt = new CallStmt( current_stmt_, callee );
  current_stmt_->addChildren( callstmt );

  return 0;
}

i32 Parser::Ret()
{
  Next();
  auto ret = new RetStmt( current_stmt_, Binary( { CurrentToken().start, CurrentToken().end } ) );
  current_stmt_->addChildren( ret );

  return 0;
}

i32 Parser::Ifer()
{
  Next();
  auto ifstmt = new IfStmt( current_stmt_, Binary( { CurrentToken().start, CurrentToken().end } ) );
  current_stmt_->addChildren( ifstmt );
  current_stmt_ = ifstmt;
  scope_ = scope_->getNextChild();

  return 0;
}

i32 Parser::Loop()
{
  Next();
  auto lstmt = new LoopStmt( current_stmt_, Binary( { CurrentToken().start, CurrentToken().end } ) );
  current_stmt_->addChildren( lstmt );
  current_stmt_ = lstmt;
  scope_ = scope_->getNextChild();

  return 0;
}

i32 Parser::Binary()
{
  current_stmt_->addChildren( Binary( { CurrentToken().start, CurrentToken().end } ) );

  return 0;
}

// pos: p_token func name lies
CallExpr::pointer Parser::Callee( u32 pos )
{
  // auto &token = (PeekPrtat(Peekat(pos).start));
  auto func = scope_->getFunc( ( PeekPrtat( Peekat( pos ).start ) ).name );
  auto callexpr = new class CallExpr( func );
  callexpr->setLevel( Level( func->getSrcloc().token ) );
  if(P_TOKEN::CALL == Peekat(pos+1).tk) pos++;
  for ( auto i { 1u }; i <= func->getParamSize() || func->ty_.isUseValist_; i++ ) {
    auto start { Peekat( pos + i ).start };
    callexpr->addPara( ParseBinary( start, Peekat( pos + i ).end ) );
    Next();
    if ( Peek() != P_TOKEN::BINARY )
      break;
  }
  return callexpr;
}

BinStmt::pointer Parser::Binary( range_t r )
{
  auto bstmt = new BinStmt( current_stmt_ );
  auto start = r.start;
  auto root { ParseBinary( start, r.end ) };
  bstmt->setRoot( root );
  return bstmt;
}

BinExpr::Node::pointer Parser::ParseBinary( u32& offset, const u32 end )
{
  // this method has bug in some expr with single op like `2+ ~3` and `- !3`,
  // however, you can use `()` to avoid this in other testcase which only cares result from ParseBinary
  // this comment maybe history now, if no bugs found in the future 5/14/23

  auto root = BinStmt::nodeCreator( PeekPrtat( offset ), scope_ );
  if ( offset + 1 >= end )
    return root;

  bool last_is_op { false };
  u32 last_offset = offset;
  while ( true ) {
    auto current = BinStmt::nodeCreator( PeekPrtat( offset ), scope_ );
    offset++;
    auto tk = current->data->getToken();
    switch ( token_type( tk.token ) ) {
      case TOKEN_TYPE::BRAC: {
        //[[fallthrough]] {
        // `fallthrough` attribute only applies to empty statement
        if ( TOKEN::NRBRAC == tk.token ) {
          root->data->setLevel( Level( TOKEN::SYMBOL ) );
          return root;
        } else {
          if ( TOKEN_TYPE::BRAC == token_type( root->data->getToken().token ) ) {
            root = ParseBinary( offset, end );
          } else {
            current = ParseBinary( offset, end );
            last_is_op = false;
            goto I_DONOT_LIKE_THIS;
          }
        }

        last_is_op = false;
        break;
      }
      I_DONOT_LIKE_THIS:
      case TOKEN_TYPE::VALUE: {
        if ( current->data->getToken().token == TOKEN::SYMBOLF ) {
          Next();
          u32 last_offset { p_offset_ };
          current->data = Callee( p_offset_ );
          for ( auto i { last_offset }; i <= p_offset_; i++ )
            offset += Peekat( i ).end - Peekat( i ).start + 1;
          offset--;
        }
        if ( token_type( root->data->getToken().token ) == TOKEN_TYPE::VALUE ) {
          // for binexpr starts with value
          root = current;
          break;
        }
        auto temp
          = root->FindChildR( []( const BinExpr::Node::pointer _node ) { return nullptr == _node->right; } );
        temp->InsertRight( current );

        last_is_op = false;
        break;
      }
      case TOKEN_TYPE::OPERATOR: {
        if ( last_offset + 1 == offset )
          break;
        if ( last_is_op )
          current->data->setLevel( Level( TOKEN::SYMBOL ) ); //--

        if ( current->data->getLevel() >= /*do not modify it to >*/ root->data->getLevel() ) {
          root->ReplaceByL( current );
          root = current;
        } else {
          bool level_flag { false };
          auto temp = root->FindChildR( [&current, &level_flag]( const BinExpr::Node::pointer _node ) {
            level_flag = ( _node->data->getLevel() > current->data->getLevel() ); /*do not modify it to <*/
            return ( nullptr == _node->right ) || ( _node->data->getLevel() <= current->data->getLevel() );
          } );
          if ( temp != root ) {
            if ( level_flag ) {
              if ( nullptr == temp->left )
                temp->InsertLeft( current );
              else
                temp->InsertRight( current );
            } else
              temp->ReplaceByL( current );
          } else
            temp->InsertRight( current );
        }

        current->data->setLevel( Level( current->data->getToken().token ) ); //--
        last_is_op = true;
        break;
      }

      default:
        return nullptr;
    }

    if ( offset >= end )
      break;
  }

  switch ( token_type( root->data->getToken().token ) ) {
    case TOKEN_TYPE::VALUE: {
    }
    case TOKEN_TYPE::OPERATOR:
      break;
    default:
      break;
  }
  return root;
}

void Parser::BfsWalk()
{
  std::queue<Stmt::pointer> que;
  std::queue<u32> quee;
  que.push( current_stmt_ );
  quee.push( 1 );
  while ( !que.empty() ) {
    auto stmt = que.front();
    que.pop();
    auto level = quee.front();
    quee.pop();
    print( "level:{}, name:{}\n", level, stmt->for_test_getName() );
    stmt->walk();
    for ( auto&& child : stmt->getChildren() ) {
      que.push( child );
      quee.push( level + 1 );
    }
  }
}

void Parser::DfsWalk()
{
  doDfsWalk( current_stmt_ );
}

void Parser::doDfsWalk( Stmt::pointer stmt, u32 level )
{
  print( "stmt_name: {}\n", stmt->for_test_getName() );
  stmt->walk();
  print( stmt->getChildren().size() == 0 ? "" : "children_size: {}\n", stmt->getChildren().size() );
  auto count { 1u };
  for ( auto&& child : stmt->getChildren() ) {
    print( "{}.{}:\n", level, count++ );
    doDfsWalk( child, level + 1 );
  }
}

} // namespace vastina