#include "codegen.hpp"
#include "base/io.hpp"
#include "base/log.hpp"
#include "expr.hpp"
#include "stmt.hpp"

#include <fcntl.h>
#include <ranges>
#include <string_view>

namespace vastina {

void Generator::Generate( const string_view& file_name )
{
  ::system( std::format( "rm -f {}", file_name ).data() );
  ::system( std::format( "touch {}", file_name ).data() );
  // todo remove this

  filer_ = new Filer( file_name );
  filer_->Open();

  filer_->PushBack( x86::file_start( file_name ) );
  EXCEPT_ZERO( doGenerate, current_stmt_ );
  filer_->PushBack( x86::asm_end_str );

  writer()->WriteAll();
  filer_->Close();
}

i32 Generator::doGenerate( Stmt::pointer stmt )
{
  static u32 pos;
  switch ( stmt->StmtType() ) {
    case STMTTYPE::Return: {
      Binary( dynamic_cast<BinStmt::pointer>( stmt->getResult() ) );
      filer_->PushBack( x86::Threer( x86::movq, x86::regIndirect( "8", x86::rsp ), x86::rax ) );
      filer_->PushBack( x86::Threer( x86::addq, x86::constant( 16 ), x86::rsp ) );
      break;
    }
    case STMTTYPE::Fdecl: {
      if ( nullptr != stmt->getParent() ) {
        if ( STMTTYPE::Fdecl == stmt->getParent()->StmtType() ) {
          // todo, func declared in func
        }
      }
      pos = filer_->PushBack( x86::func_declare_start( stmt->getFunc()->getName() ) );
      filer_->PushBack( x86::func_start( stmt->getFunc()->getName(), counter_.lf.lfbe ) );
      break;
    }
    case STMTTYPE::Vdecl: {
      auto type { stmt->getVar()->getType() };
      switch ( type ) {
        case TOKEN::INT:
          break;

        case TOKEN::LONG:

          break;
        default:
          THIS_NOT_SUPPORT( std::format( "type with token id {},", (i32)type ) );
      }
      break;
    }
    case STMTTYPE::Call: {
      Callee( dynamic_cast<CallStmt::pointer>( stmt ) );
      break;
    }
    case STMTTYPE::Binary: {
      Binary( dynamic_cast<BinStmt::pointer>( stmt ) );
    }
    default:
      break;
  }

  for ( auto&& child : stmt->getChildren() ) {
    doGenerate( child );
  }

  switch ( stmt->StmtType() ) {
    case STMTTYPE::Fdecl: {
      if ( stmt->getFunc()->getSrcloc().token == TOKEN::MAIN )
        filer_->PushBack( x86::main_func_end );
      else
        filer_->PushBack( x86::func_end );
      filer_->PushBack( x86::func_declare_end( counter_.lf.lfbe++, stmt->getFunc()->getName() ) );
    }
    case STMTTYPE::Compound:
    case STMTTYPE::Vdecl:
    default:
      break;
  }

  return 0;
}

void Generator::Callee( CallStmt::pointer stmt )
{
  auto callee { dynamic_cast<CallExpr::pointer>( stmt->getData() ) };

  return doCallee( callee );
}

void Generator::doCallee( CallExpr::pointer callee )
{
  auto paras { callee->getParas() };
  auto func { callee->getFunc() };

  auto pos { paras.size() - 1 };
  u32 stack_usage {};
  for ( ; pos > 5; pos-- ) {

    switch ( paras.at( pos )->getToken() ) {
      case TOKEN::STRING: {
        filer_->Insert(
          counter_.loc.pos++,
          x86::rodata( counter_.loc.lc++,
                       "string",
                       std::ranges::to<std::string>( paras.at( pos )->getRoot()->data->getName()
                                                     | std::views::filter( []( char c ) {
                                                         return c != '\n';
                                                       } ) ) ) ); // name means data from original buffer
        filer_->PushBack( x86::Threer(
          x86::leaq, x86::regIndirect( std::format( ".LC{}", counter_.loc.lc - 1 ), x86::rip ), x86::rax ) );
        {
          filer_->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
          stack_usage += 8;
        }
        break;
      }
      case TOKEN::LCHAR: {
        // if you make two or more letters in '', it will report error before this
        char ch = paras.at( pos )->getName()[1];
        filer_->PushBack( x86::Twoer( x86::pushq, x86::constant( ch - 0 ) ) );
        stack_usage += 8;
        break;
      }
      case TOKEN::CHAR:
      case TOKEN::INT:
      case TOKEN::LONG: {
      }
      case TOKEN::FLOAT:
      case TOKEN::DOUBLE:
      default:
        THIS_NOT_SUPPORT( std::format( "type with token id {},", (i32)paras.at( pos )->getToken() ) );
    }
  }
  do {
    switch ( paras.at( pos )->getToken() ) {
      case TOKEN::STRING: {
        filer_->Insert( counter_.loc.pos++,
                        x86::rodata( counter_.loc.lc++,
                                     "string",
                                     std::ranges::to<std::string>(
                                       paras.at( pos )->getRoot()->data->getName()
                                       | std::views::filter( []( char c ) { return c != '\n'; } ) ) ) );
        filer_->PushBack( x86::Threer( x86::leaq,
                                       x86::regIndirect( std::format( ".LC{}", counter_.loc.lc - 1 ), x86::rip ),
                                       x86::regs_for_call[pos] ) );
        break;
      }
      case TOKEN::SYMBOL:
      case TOKEN::CHAR:
      case TOKEN::INT:
      case TOKEN::LONG: {
        // tobe removed
        filer_->PushBack( x86::Threer( x86::movq, x86::constant( 100 ), x86::regs_for_call[pos] ) );
        break;
      }
      case TOKEN::FLOAT:
      case TOKEN::DOUBLE:
      default:
        break;
    }
  } while ( pos-- );

  if ( func->isBuiltin_ )
    filer_->PushBack( x86::call_builtin( func->getName() ) );
  else
    filer_->PushBack( x86::make_call( func->getName() ) );

  if ( stack_usage > 0 ) {
    filer_->PushBack( x86::Threer( x86::addq, std::format( "${}", stack_usage ), x86::rsp ) );
  }
}

void Generator::Binary( BinStmt::pointer stmt )
{
  auto data { stmt->getData()->getRoot() };

  return doBinary( data );
}

void Generator::doBinary( BinExpr::Node::pointer node )
{
  const static auto helper { [this]( BinExpr::Node::pointer node, const std::function<void()>& details ) {
    doBinary( node->left );
    doBinary( node->right );
    poper( trr_ );
    poper( tlr_ );

    details();

    pusher( x86::rax );
  } };
  const static auto single_op { [this]( BinExpr::Node::pointer toTravel, const std::function<void()> details ) {
    doBinary( toTravel );
    details();
    pusher( x86::rax );
  } };

  auto tk { node->data->getToken() };
  switch ( token_type( tk ) ) {
    case TOKEN_TYPE::OPERATOR: {
      switch ( tk ) {
        case TOKEN::GREATER: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, trr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setg, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
          } );
        }
        case TOKEN::GREATEREQUAL: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, trr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setge, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
          } );
        }
        case TOKEN::LESS: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, trr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setl, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
          } );
        }
        case TOKEN::LESSEQUAL: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, trr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setle, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
          } );
        }
        case TOKEN::XOR: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::xorq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::LSHIFT: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::shlq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::RSHIFT: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::shrq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::OR: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::orq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::AND: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::andq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::LOGNOT: {
          // not good
          return nullptr == node->left ? single_op( node->right,
                                                    [this] {
                                                      poper( trr_ );
                                                      writer()->PushBack( x86::Threer( x86::testq, trr_, trr_ ) );
                                                      writer()->PushBack( x86::to_zero( x86::rax ) );
                                                      writer()->PushBack( x86::Twoer( x86::sete, x86::al ) );
                                                    } )
                                       : single_op( node->left, [this] {
                                           poper( tlr_ );
                                           writer()->PushBack( x86::Threer( x86::testq, tlr_, tlr_ ) );
                                           writer()->PushBack( x86::to_zero( x86::rax ) );
                                           writer()->PushBack( x86::Twoer( x86::sete, x86::al ) );
                                         } );
          // so how to tell clang-format not do this?
        }
        case TOKEN::LOGOR: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::orq, trr_, tlr_ ) );
            // writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
            writer()->PushBack( x86::Threer( x86::testq, tlr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setne, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
          } );
        }
        case TOKEN::LOGAND: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::testq, tlr_, tlr_ ) );
            writer()->PushBack( x86::Twoer( x86::setne, x86::r13b ) );
            writer()->PushBack( x86::Threer( x86::testq, trr_, trr_ ) );
            writer()->PushBack( x86::Twoer( x86::setne, x86::al ) );
            writer()->PushBack( x86::Threer( x86::movzbl, x86::al, x86::eax ) );
            writer()->PushBack( x86::Threer( x86::andl, x86::r13d, x86::eax ) );
          } );
        }
        case TOKEN::EQUAL: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, tlr_, trr_ ) );
            writer()->PushBack( x86::to_zero( x86::rax ) );
            writer()->PushBack( x86::Twoer( x86::sete, x86::al ) );
          } );
        }
        case TOKEN::NOTEQUAL: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::cmpq, tlr_, trr_ ) );
            writer()->PushBack( x86::to_zero( x86::rax ) );
            writer()->PushBack( x86::Twoer( x86::setne, x86::al ) );
          } );
        }
        case TOKEN::PLUS: {
          if ( nullptr == node->left or nullptr == node->right ) {
            const static auto donothing { [this] { poper( x86::rax ); } };
            return nullptr == node->left ? single_op( node->right, donothing ) : single_op( node->left, donothing );
          }
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::addq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::NEG: {
          if ( nullptr == node->left and nullptr != node->right ) {
            return single_op( node->right, [this] {
              poper( trr_ );
              writer()->PushBack( x86::to_neg( trr_ ) );
              writer()->PushBack( x86::Threer( x86::movq, trr_, x86::rax ) );
            } );
          } else if ( nullptr == node->right and nullptr != node->left ) {
            return single_op( node->left, [this] {
              poper( tlr_ );
              writer()->PushBack( x86::to_neg( tlr_ ) );
              writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
            } );
          }
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::subq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::ASSIGN: {
          // should be replaced by data location
          // auto des {node->left->data->getName()};
          doBinary( node->right );
          poper( x86::rax );
          return; //(void)writer()->PushBack( x86::Threer( x86::movq, x86::rax, "-8(%rsp)" ) );
        }
        case TOKEN::MULTI: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::imulq, trr_, tlr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
          } );
        }
        case TOKEN::DIV: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
            writer()->PushBack( x86::single( x86::cqto ) );
            writer()->PushBack( x86::Twoer( x86::idivq, trr_ ) );
          } );
        }
        case TOKEN::MOD: {
          return helper( node, [this] {
            writer()->PushBack( x86::Threer( x86::movq, tlr_, x86::rax ) );
            writer()->PushBack( x86::Threer( x86::xorq, x86::rdx, x86::rdx ) );
            writer()->PushBack( x86::single( x86::cqto ) );
            writer()->PushBack( x86::Twoer( x86::idivq, trr_ ) );
            writer()->PushBack( x86::Threer( x86::movq, x86::rdx, x86::rax ) );
          } );
        }
        case TOKEN::OPS: {
          const auto details { [this] {
            poper( x86::rax );
            writer()->PushBack( x86::Twoer( x86::notq, x86::rax ) );
          } };
          return nullptr == node->right ? single_op( node->left, details ) : single_op( node->right, details );
        }
        default:
          THIS_NOT_SUPPORT( node->data->getName() );
          print( "\nbye\n" );
          exit( 0 );
      }
    }
    case TOKEN_TYPE::VALUE: {
      switch ( tk ) {
        case TOKEN::SYMBOLF: {
          auto callee { dynamic_cast<CallExpr::pointer>( node->data ) };
          doCallee( callee );
          if ( !callee->getFunc()->isVoid_ ) {
            pusher( x86::rax );
            // filer_->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
          }
          return;
        }
        case TOKEN::NUMBER: {
          // assume all integer here
          auto val { std::stoi( node->data->getName().data() ) };
          writer()->PushBack( x86::Threer( x86::movq, std::format( "${}", val ), x86::rax ) );

          return pusher( x86::rax );
          // return (void)writer()->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
        }
        case TOKEN::STRING:

        case TOKEN::TRUE:

        case TOKEN::FALSE:

        default:
          THIS_NOT_SUPPORT( node->data->getName() );
          print( "\nbye\n" );
          exit( 0 );
      }
    }
    default:
      THIS_NOT_SUPPORT( node->data->getName() );
      exit( 1 );
  }
}

void Generator::poper( const string_view& reg )
{
  filer_->PushBack( x86::Threer( x86::movq, x86::regIndirect( "8", x86::rsp ), reg ) );
  filer_->PushBack( x86::Threer( x86::addq, x86::constant( 16 ), x86::rsp ) );
}

void Generator::pusher( const string_view& reg )
{
  filer_->PushBack( x86::Threer( x86::subq, x86::constant( 16 ), x86::rsp ) );
  filer_->PushBack( x86::Threer( x86::movq, reg, x86::regIndirect( "8", x86::rsp ) ) );
}

}; // namespace vastina
