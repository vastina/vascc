#include "base/x86.hpp"
#include "base/log.hpp"

namespace vastina {

std::string x86::to_lower( std::string reg )
{
  // r8 -> r15
  if ( CharType( reg[1] ) == CHARTYPE::NUM ) {
    if ( reg.size() == 3 )
      return std::string( reg ).append( "d" );
    switch ( char { reg.back() } ) {
      case 'd':
        return reg.substr( 0, 3 ).append( "w" );
      case 'w':
        return reg.substr( 0, 3 ).append( "b" );
      case 'b':
      default:
        LEAVE_MSG( "reg is already the lowest" );
        return reg;
    }
  }
  // rxx, exx
  switch ( reg[0] ) {
    case 'r':
      return std::string( "e" ).append( ( reg.substr( 1 ) ) );
    case 'e':
      return reg.substr( 1 );
    default:
      break;
  }
  // ax, di, sp -> al, dil, spl
  switch ( reg[1] ) {
    case 'x':
      return reg.substr( 0, 1 ).append( "l" );
    case 'i':
    case 'p':
      return reg.append( "l" );
    default:
      break;
  }
  throw std::runtime_error( "what the fucking hell reg you sent?" );
}

std::string x86::file_start( const string_view& filename )
{
  return format( file_start_, filename );
}

std::string x86::func_declare_start( const string_view& func_name )
{
  return format( func_declare_start_, func_name, func_name );
}

std::string x86::func_start( const string_view& func_name, u32 counter )
{
  return format( func_start_, func_name, counter );
}

std::string x86::func_declare_end( u32 counter, const string_view& func_name )
{
  return format( func_declare_end_, counter, func_name, func_name );
}

std::string x86::constant( i64 val )
{
  return format( constant_, val );
}

std::string x86::Threer( const string_view& op, const string_view& _1, const string_view& _2 )
{
  return format( Threer_, op, _1, _2 );
}

std::string x86::Twoer( const string_view& op, const string_view& _1 )
{
  return format( Two_1, op, _1 );
}

std::string x86::Twoer_2( const string_view& op, const string_view& _1 )
{
  return format( Two_2, op, _1 );
}

std::string x86::make_call( const string_view& func_name )
{
  return Twoer( call, func_name );
}

std::string x86::call_builtin( const string_view& func_name )
{
  return Twoer( call, std::format( "{}@PLT", func_name ) );
}

std::string x86::make_jump( const string_view& op, const string_view& location )
{
  return Twoer( op, location );
}

std::string x86::regIndirect( const string_view& val, const string_view& reg )
{
  return format( regIndirect_, val, reg );
}

std::string x86::rodata( u32 counter, const string_view& type, const string_view& data )
{
  return format( rodata_, counter, type, data );
}

std::string x86::single( const string_view& op )
{
  return format( single_, op );
}

std::string x86::leaer( const string_view& val,
                        const string_view& reg1,
                        const string_view& reg2,
                        const string_view& time )
{
  return format( lea_er_, val, reg1, reg2, time );
}

std::string x86::to_neg( const string_view& reg )
{
  return format( to_neg_, reg, reg );
}

std::string x86::to_zero( const string_view& reg )
{
  return format( to_zero_, reg, reg );
}

};