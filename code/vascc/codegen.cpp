#include "codegen.hpp"
#include "base/Tree.hpp"
#include "base/io.hpp"
#include "base/log.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "symbol.hpp"

#include <ranges>

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
  switch ( stmt->StmtType() ) {
    case STMTTYPE::Return: {
      Binary( dynamic_cast<BinStmt::pointer>( stmt->getResult() ), false );
      poper( x86::rax );
      // todo clean
      // filer_->PushBack(x86::single(x86::ret));
      break;
    }
    case STMTTYPE::Fdecl: {
      FuncStart( dynamic_cast<FdeclStmt::pointer>(stmt));
      break;
    }
    case STMTTYPE::Vdecl: {
      Vdecl(dynamic_cast<VdeclStmt::pointer>(stmt));
      break;
    }
    case STMTTYPE::Call: {
      Callee( dynamic_cast<CallStmt::pointer>( stmt ) );
      break;
    }
    case STMTTYPE::Binary: {
      Binary( dynamic_cast<BinStmt::pointer>( stmt ), true );
    }
    case STMTTYPE::If: {

    }
    case STMTTYPE::Loop: {

    }
    case STMTTYPE::Cond:
    case STMTTYPE::Compound:
    default:
      break;
  }

  for ( auto&& child : stmt->getChildren() ) {
    doGenerate( child );
  }

  switch ( stmt->StmtType() ) {
    case STMTTYPE::Fdecl: {
      FuncEnd(dynamic_cast<FdeclStmt::pointer>(stmt));
    }
    case STMTTYPE::Binary:
    case STMTTYPE::Call:
    case STMTTYPE::Compound:
    case STMTTYPE::Vdecl:
    case STMTTYPE::If:
    case STMTTYPE::Loop:
    default:
      break;
  }

  return 0;
}

void Generator::FuncStart( FdeclStmt::pointer stmt ){
  static u32 func_pos {};

  // it always has a parent named CompoundStmt at least
  // if ( nullptr != stmt->getParent() )
  if ( STMTTYPE::Fdecl == stmt->getParent()->StmtType() ) {
    // todo, func declared in func
  }
  func_pos = filer_->PushBack( x86::func_declare_start( stmt->getFunc()->getName() ) );
  filer_->PushBack( x86::func_start( stmt->getFunc()->getName(), counter_.lf.lfbe ) );
  Params( stmt->getFunc()->getParams() );
}

void Generator::FuncEnd( FdeclStmt::pointer stmt ){
  if ( stmt->getFunc()->getSrcloc().token == TOKEN::MAIN ) {
    filer_->PushBack( x86::main_func_end );
  } else {
    ParamClean( stmt->getFunc()->getParams() );
    filer_->PushBack( x86::func_end );
  }
  filer_->PushBack( x86::func_declare_end( counter_.lf.lfbe++, stmt->getFunc()->getName() ) );
}

void Generator::Vdecl( VdeclStmt::pointer stmt ){
  if(nullptr == scope_->getParent()){
    // todo, if is global here
  }

  const auto var { stmt->getVar() };
      if ( var->ty_.isParam )
        return;

      auto type { var->getType() };
      switch ( type ) {
        case TOKEN::CHAR:
        case TOKEN::INT:
        case TOKEN::LONG: {
          if ( var->ty_.isConst ) {}
          if ( var->ty_.isStatic ) {}
          if ( var->ty_.isPointer ) {}
          if ( nullptr != stmt->getIniter() ) {
            Binary( dynamic_cast<BinStmt::pointer>( stmt->getIniter() ), false );
          } else {
            pusher( x86::rax ); // random value in rax
          }
          var->stack.offset = counter_.rsp - sizeof( long ); // -offset(%rbp)
          break;
        }
        default:
          THIS_NOT_SUPPORT( std::format( "type with token id {},", (i32)type ) );
      }
}

void Generator::Params( const std::vector<Variable::pointer>& paras )
{
  if ( paras.empty() )
    return;

  auto pos { paras.size() - 1 };
  for ( ; pos > 5; pos-- ) {}
  do {
    pusher( x86::regs_for_call[pos] );
    paras.at( pos )->stack.offset = counter_.rsp - sizeof( long );
  } while ( pos-- );
}

void Generator::ParamClean( const std::vector<Variable::pointer>& paras )
{
  if ( paras.empty() )
    return;

  auto pos { paras.size() - 1 };
  for ( ; pos > 5; pos-- ) {}
  do {
    poper( x86::r10 ); // 随便选的
  } while ( pos-- );
}

void Generator::Callee( CallStmt::pointer stmt )
{
  auto callee { dynamic_cast<CallExpr::pointer>( stmt->getData() ) };

  return doCallee( callee );
}

void Generator::doCallee( CallExpr::pointer callee )
{
  const auto paras { callee->getParas() };
  const auto func { callee->getFunc() };
  
  u32 stack_usage {};
  if(!paras.empty()) {
  auto pos { paras.size() - 1 };
  for ( ; pos > 5; pos-- ) {
    doBinary( paras.at( pos )->getRoot() ); // Binary.pop == flase
    poper(x86::rax);
    filer_->PushBack(x86::Twoer(x86::pushq, x86::rax));
    stack_usage += 8; // this is wrong
  }
  do {
    doBinary( paras.at( pos )->getRoot() ); // Binary.pop == flase
    poper( x86::regs_for_call[pos] );
  } while ( pos-- );
  }
  if ( func->ty_.isBuiltin_ )
    filer_->PushBack( x86::call_builtin( func->getName() ) );
  else
    filer_->PushBack( x86::make_call( func->getName() ) );

  if ( stack_usage > 0 ) {
    filer_->PushBack( x86::Threer( x86::addq, std::format( "${}", stack_usage ), x86::rsp ) );
  }
}

void Generator::Binary( BinStmt::pointer stmt, bool pop )
{
  const auto data { stmt->getData()->getRoot() };

  doBinary( data );
  if ( pop )
    poper( x86::r10 );
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

  const auto tk { node->data->getToken() };
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
          /*                =
                           /  \
                          =   expr
                        /   \
                      var1  var2
            go deep here or do sth others?
          */
          //auto des { dynamic_cast<Variable::pointer>( node->left->data->getVal() ) };
          doBinary( node->right );
          poper( x86::rax );
          node->left->Travel(walk_order::PREORDER, [this](const decltype(node->data)& data_){
            if(TOKEN::SYMBOL == data_->getToken()){
              auto des { dynamic_cast<Variable::pointer>(data_->getVal()) };
              this->writer()->PushBack( x86::Threer(
                x86::movq, x86::rax, x86::regIndirect( std::format( "-{}", des->stack.offset ), x86::rbp ) ) );
            }
          });
          //writer()->PushBack( x86::Threer(
            //x86::movq, x86::rax, x86::regIndirect( std::format( "-{}", des->stack.offset ), x86::rbp ) ) );
          return pusher( x86::rax );
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
        case TOKEN::SYMBOL: {
          auto var { dynamic_cast<Variable::pointer>( node->data->getVal() ) };
          writer()->PushBack( x86::Threer(
            x86::movq, x86::regIndirect( std::format( "-{}", var->stack.offset ), x86::rbp ), x86::rax ) );
          return pusher( x86::rax );
        }
        case TOKEN::SYMBOLF: {
          auto callee { dynamic_cast<CallExpr::pointer>( node->data ) };
          doCallee( callee );
          if ( !callee->getFunc()->ty_.isVoid_ ) {
            pusher( x86::rax );
            // filer_->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
          }
          return;
        }
        case TOKEN::NUMBER: {
          // todo
          const auto val { std::stol( node->data->getName().data() ) };
          writer()->PushBack( x86::Threer( x86::movq, std::format( "${}", val ), x86::rax ) );

          return pusher( x86::rax );
          // return (void)writer()->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
        }
        case TOKEN::STRING:
          filer_->Insert(
            counter_.loc.pos++,
            x86::rodata( counter_.loc.lc++,
                         "string",
                         std::ranges::to<std::string>(
                           node->data->getName() | std::views::filter( []( char c ) { return c != '\n'; } ) ) ) );
          filer_->PushBack( x86::Threer(
            x86::leaq, x86::regIndirect( std::format( ".LC{}", counter_.loc.lc - 1 ), x86::rip ), x86::rax ) );
          return pusher( x86::rax );
        case TOKEN::LCHAR: {
          // if you make two or more letters in '', it will report error before this
          char ch = node->data->getName()[1];
          filer_->PushBack( x86::Twoer( x86::pushq, x86::constant( ch - 0 ) ) );
          break;
        }
        case TOKEN::TRUE:

        case TOKEN::FALSE:

        default:
          THIS_NOT_SUPPORT( node->data->getName() );
          print( "\nbye\n" );
          std::exit( -1 );
      }
      throw 114514;
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
  counter_.rsp -= 16;
}

void Generator::pusher( const string_view& reg )
{
  filer_->PushBack( x86::Threer( x86::subq, x86::constant( 16 ), x86::rsp ) );
  filer_->PushBack( x86::Threer( x86::movq, reg, x86::regIndirect( "8", x86::rsp ) ) );
  counter_.rsp += 16;
}

}; // namespace vastina
