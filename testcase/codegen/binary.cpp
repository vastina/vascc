#include "base/x86.hpp"
#include "base/io.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "codegen.hpp"
#include "expr.hpp"
#include "lexer.hpp"
#include "parse.hpp"
#include "symbol.hpp"

#include <folly/Function.h>
#include <iostream>
#include <memory>
#include <string>

using namespace vastina;

auto writer { new Writer( "binary.s" ) };
auto tlr { x86::r15 }; // temp left reg
auto trr { x86::r14 }; //      right

void binary( BinStmt::pointer );
void doBinary( BinExpr::Node::pointer node );

int main( int argc, char* argv[] )
{
  if ( argc != 2 ) {
    std::cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  lexer lx { lexer( argv[1] ) };
  lx.Parse();

  auto tks { std::make_unique<std::vector<token_t>>( lx.getTokens() ) };
  Preprocess* pp { new Preprocess( *tks, lx.getScope() ) };
  pp->Process();

  auto psr { new Parser( *tks.get(), pp->getResult(), pp->CurrentScope() ) };
  (void)psr->Parse();
  // psr->DfsWalk();

  writer->Open();
  //------------------------------
  writer->PushBack( x86::file_start( "binary" ) );
  writer->PushBack( x86::rodata( 0, "string", std::format( "\"{}\"", "use echo $?" ) ) );
  writer->PushBack( x86::func_declare_start( "main" ) );
  writer->PushBack( x86::func_start( "main", 0 ) );
  writer->PushBack( x86::Threer( x86::leaq, x86::regIndirect( std::format( ".LC{}", 0 ), x86::rip ), x86::rax ) );
  writer->PushBack( x86::Threer( x86::movq, x86::rax, x86::rdi ) );
  writer->PushBack( x86::make_call( "puts@PLT" ) );
  // test code looks like:
  //  int a;   //need a declare to avoid exit_error, but no error when doing assign at global...
  //  a = ...; //some binary expr contains a;
  binary( dynamic_cast<BinStmt::pointer>( psr->getStmtRoot()->getChildren().at( 1 ) ) );
  writer->PushBack( x86::Threer( x86::movq, "-8(%rsp)", x86::rax ) );

  writer->PushBack( x86::main_func_end );
  writer->PushBack( x86::func_declare_end( 0, "main" ) );
  writer->PushBack( x86::asm_end_str );
  //------------------------------
  writer->WriteAll();
  writer->Close();

  return 0;
}

constexpr auto helper { []( BinExpr::Node::pointer node, const folly::Function<void()>& details ) {
  doBinary( node->left );
  doBinary( node->right );
  writer->PushBack( x86::Twoer( x86::popq, trr ) );
  writer->PushBack( x86::Twoer( x86::popq, tlr ) );

  const_cast<folly::Function<void()>&>( details )();

  return (void)writer->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
} };

void doBinary( BinExpr::Node::pointer node )
{
  auto tk { node->data->getToken() };
  switch ( token_type( tk ) ) {
    case TOKEN_TYPE::OPERATOR: {
      switch ( tk ) {
        case TOKEN::XOR: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::xorq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::LSHIFT: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::shlq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::RSHIFT: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::shrq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::OR: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::orq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::LOGOR: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::orq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
            writer->PushBack( x86::Twoer( x86::test, x86::rax ) );
          } );
        }
        case TOKEN::LOGAND: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::andq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::EQUAL: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::cmpq, tlr, trr ) );
            writer->PushBack( x86::Threer( x86::movq, std::format( "${}", 0 ), x86::rax ) );
            writer->PushBack( x86::Twoer( x86::sete, x86::al ) );
          } );
        }
        case TOKEN::NOTEQUAL: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::cmpq, tlr, trr ) );
            writer->PushBack( x86::Threer( x86::movq, std::format( "${}", 0 ), x86::rax ) );
            writer->PushBack( x86::Twoer( x86::setne, x86::al ) );
          } );
        }
        case TOKEN::PLUS: {
          if ( nullptr == node->left or nullptr == node->right ) {
            // so todo
          }
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::addq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::NEG: {
          if ( nullptr == node->left or nullptr == node->right ) {}
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::subq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::ASSIGN: {
          // should be replaced by data location
          // auto des {node->left->data->getName()};
          doBinary( node->right );
          writer->PushBack( x86::Twoer( x86::popq, x86::rax ) );
          return (void)writer->PushBack( x86::Threer( x86::movq, x86::rax, "-8(%rsp)" ) );
        }
        case TOKEN::MULTI: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::imulq, trr, tlr ) );
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
          } );
        }
        case TOKEN::DIV: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
            writer->PushBack( x86::single( x86::cqto ) );
            writer->PushBack( x86::Twoer( x86::idivq, trr ) );
          } );
        }
        case TOKEN::MOD: {
          return helper( node, [] {
            writer->PushBack( x86::Threer( x86::movq, tlr, x86::rax ) );
            writer->PushBack( x86::Threer( x86::xorq, x86::rdx, x86::rdx ) );
            writer->PushBack( x86::single( x86::cqto ) );
            writer->PushBack( x86::Twoer( x86::idivq, trr ) );
            writer->PushBack( x86::Threer( x86::movq, x86::rdx, x86::rax ) );
          } );
        }
        case TOKEN::OPS: {
          if ( nullptr == node->left )
            doBinary( node->right );
          else
            doBinary( node->left );
          writer->PushBack( x86::Twoer( x86::popq, x86::rax ) );
          writer->PushBack( x86::Twoer( x86::notq, x86::rax ) );
          return (void)writer->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
        }
        default:
          THIS_NOT_SUPPORT( node->data->getName() );
          print( "\nbye\n" );
          exit( 0 );
      }
    }
    case TOKEN_TYPE::VALUE: {
      switch ( tk ) {
        case TOKEN::NUMBER: {
          // assume all integer here
          auto val { std::stoi( node->data->getName().data() ) };
          writer->PushBack( x86::Threer( x86::movq, std::format( "${}", val ), x86::rax ) );
          return (void)writer->PushBack( x86::Twoer( x86::pushq, x86::rax ) );
        }
        case TOKEN::STRING:

        case TOKEN::TRUE:

        case TOKEN::FALSE:

        default:
          print( "this not support" );
          exit( 0 );
      }
    }
    default:
      THIS_NOT_SUPPORT( node->data->getName() );
      exit( 1 );
  }
}

// no call now
void binary( BinStmt::pointer stmt )
{
  auto data { stmt->getData()->getRoot() };

  return doBinary( data );
}
