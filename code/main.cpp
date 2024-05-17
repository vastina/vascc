#include "base/string.hpp"
#include "codegen.hpp"
#include "lexer.hpp"
#include "parse.hpp"
#include "symbol.hpp"

#include <iostream>
#include <memory>
#include <queue>

// const static before_main_t before_main = before_main_t();
// I know using new directly is bad

using namespace vastina;
int main( int argc, char* argv[] )
{
  if ( argc != 2 ) {
    std::cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  lexer lx = lexer( argv[1] );
  std::cout << "--------------------------lexer-----------------------------"
               "--------\n";
  lx.Parse();

  auto tks = std::make_unique<std::vector<token_t>>( lx.getTokens() );
  for ( u32 i = 0; i < tks->size(); i++ ) {
    print( "offset:{}, \ttoken:{} \ttokenid:{}, \tline:{}\n",
           i,
           tks->at( i ).name,
           (i32)tks->at( i ).token,
           tks->at( i ).line );
  }
  std::cout << "--------------------------preprocess-result-----------------"
               "--------\n";
  auto pp = Preprocess( *tks, lx.getScope() );
  pp.Process();

  for ( u32 i = 0; i < pp.getSize(); i++ ) {
    auto& next = pp.getNext();
    print( "offset: {}\nProcessedTokenType: {}\nstr:\t\"", i, p_token_str( next.tk ) );
    for ( u32 j = next.start; j < next.end; j++ ) {
      std::cout << tks->at( j ).name << ' ';
    }
    std::cout << "\"\n";
  }
  std::cout << "--------------------------symboltable--scope----------------"
               "--------\n";

  std::queue<decltype( pp.CurrentScope() )> st;
  st.push( pp.CurrentScope() );

  while ( !st.empty() ) {
    auto scope = st.front();
    st.pop();
    auto&& table = scope->getSymbolTable();
    print( "start:\t {}\nend:\t {}\nvars: ", scope->getRange().start, scope->getRange().end );
    for ( auto&& var : *( table->Variables ) )
      print( "{}, ", var.first );
    print( "\nfuns: " );
    for ( auto&& fc : *( table->functions ) )
      print( "{}, ", fc.first );
    print( "\n" );
    for ( auto&& child : scope->getChildren() )
      st.push( child );
  }

  std::cout << "--------------------------Parser----------------------------"
               "--------\n";

  auto psr { Parser( *tks.get(), pp.getResult(), pp.CurrentScope() ) };
  (void)psr.Parse();
  psr.DfsWalk();

  auto gen { Generator( psr.getStmtRoot(), pp.CurrentScope() ) };
  gen.Generate( std::string( argv[1] ).append( ".s" ) );

  return 0;
}