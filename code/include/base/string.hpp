#ifndef _BASE_STRING_H_
#define _BASE_STRING_H_

#include "base/vasdef.hpp"

#include <format>
#include <string_view>


namespace vastina {

enum class CHARTYPE
{
  CHAR,
  NUM,
  OTHER
};

inline constexpr CHARTYPE CharType( char ch )
{
  if ( ch >= '0' && ch <= '9' )
    return CHARTYPE::NUM;
  if ( ( ch >= 'a' && ch <= 'z' ) || ( ch == '_' ) || ( ch >= 'A' && ch <= 'Z' ) )
    return CHARTYPE::CHAR;
  return CHARTYPE::OTHER;
}

inline constexpr bool isWhiteSpace( char ch )
{
  return ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );
}

inline bool Strcmp( const std::string& buffer, u32 offset, const string_view& temp )
{
  u32 len = temp.size();
  for ( u32 i = 0; i < len; i++ ) {
    if ( buffer.at( i + offset ) != temp.at( i ) )
      return false;
  }
  return true;
}

inline const_str_t p_token_str( P_TOKEN ptk )
{
  switch ( ptk ) {
    case P_TOKEN::BINARY:
      return "binary";
    case P_TOKEN::FDECL:
      return "func declare";
    case P_TOKEN::VDECL:
      return "var declare";
    case P_TOKEN::ADDR:
      return "address";
    case P_TOKEN::IF:
      return "if";
    case P_TOKEN::LOOP:
      return "loop";
    case P_TOKEN::CALL:
      return "call";
    case P_TOKEN::RET:
      return "return";
    case P_TOKEN::END:
      return "end";
    default:
      return {};
  }
}

inline constexpr TOKEN _TypeToken( string_view sv )
{
  if ( sv.size() == 0 )
    return TOKEN::UNKNOW;

  switch ( sv[0] ) {
    case 'i':
      return TOKEN::INT;
    case 'j':
      return TOKEN::UNSIGNED;
    case 'd':
      return TOKEN::DOUBLE;
    case 'c':
      return TOKEN::CHAR;
    case 'b':
      return TOKEN::BOOL;
    case 'f':
      return TOKEN::FLOAT;
    case 'l':
      return TOKEN::LONG;
    case 'v':
      return TOKEN::VOID;
    default:
      return TOKEN::UNKNOW;
  }
}

template<typename ty>
inline constexpr TOKEN Type()
{
  return _TypeToken( typeid( ty ).name() );
}

template<typename... Args>
void print( const string_view& fmt_str, Args&&... args )
{
  // auto fmt_args{ std::make_format_args(args...) };
  // std::string outstr{ std::vformat(fmt_str, fmt_args) };
  fputs( std::vformat( fmt_str, std::make_format_args( args... ) ).c_str(), stdout );
};

template<typename... Args>
std::string format( const std::string_view& fmt_str, Args&&... args )
{
  return std::vformat( fmt_str, std::make_format_args( args... ) );
}

} // namespace vastina

#endif