#ifndef _LEXER_H_
#define _LEXER_H_

#include "base/vasdef.hpp"
#include "symbol.hpp" // see symbol.hpp struct token_t

#include <functional>
#include <string>
#include <vector>

namespace vastina {

class lexer
{

public:
  // only local use enum
  enum STATE
  {
    END = -1,
    NORMAL,
    ERROR
  };
  enum RESULT
  {
    SUCCESS,
    FAIL
  };
  // todo: this should be removed in serval commits

private:
  std::vector<token_t> tokens;
  std::string buffer;
  u32 offset;
  u32 line;
  u32 lineoffset; // todo
  STATE state;

  Scope::pointer current_scope;

  STATE ParseWhiteSpace();
  RESULT ParseKeyWord( const string_view&,
                       TOKEN,
                       const std::function<bool( char )>&,
                       TOKEN Default,
                       const std::function<bool( char )>& );
  // todo: parse number
  void ParseNumber();
  void forSingelWord( const string_view& target, TOKEN target_type );

public:
  lexer( const char* filename );
  ~lexer();

  i32 Parse();

  STATE Next();
  void NextLine();
  i32 reScan(); // maybe need in the futrue

  const std::vector<token_t>& getTokens();
  const string_view getBuffer();

  // this is not good
  Scope::pointer getScope();
};

} // namespace vastina

#endif