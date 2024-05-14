#include "base/x86.hpp"
#include "base/string.hpp"

namespace vastina {

std::string x86::file_start( const string_view& filename )
{
  return fmt::format( file_start_, filename ); // so this works too
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

};