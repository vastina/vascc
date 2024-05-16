#include "base/io.hpp"
#include "base/vasdef.hpp"
#include "base/x86.hpp"
#include <format>

using namespace vastina;
int main( int argc, char* argv[] )
{
  (void)system( "rm -f hello.s" );
  (void)system( "touch hello.s" );
  auto filer { new Filer( "hello.s" ) };
  auto writer { &filer->writer() };

  std::string greet { "hello " };
  if ( 1 == argc )
    greet.append( "world" );
  else {
    for ( auto i { 1 }; i < argc; i++ ) {
      greet.append( argv[i] );
    }
  }

  writer->PushBack( x86::file_start( "hello" ) );
  u32 count { 0 };
  writer->PushBack( x86::rodata( count, "string", std::format( "\"{}\"", greet ) ) );
  writer->PushBack( x86::func_declare_start( "main" ) );
  writer->PushBack( x86::func_start( "main", 0 ) );
  writer->PushBack(
    x86::Threer( x86::leaq, x86::regIndirect( std::format( ".LC{}", count ), x86::rip ), x86::rax ) );
  writer->PushBack( x86::Threer( x86::movq, x86::rax, x86::rdi ) );
  writer->PushBack( x86::make_call( "puts@PLT" ) );
  writer->PushBack( x86::Threer( x86::movl, "$0", x86::eax ) );
  writer->PushBack( x86::main_func_end );
  writer->PushBack( x86::func_declare_end( 0, "main" ) );
  writer->PushBack( x86::asm_end_str );

  writer->Open();
  writer->WriteAll();
  writer->Close();

  delete filer;
  return 0;
}