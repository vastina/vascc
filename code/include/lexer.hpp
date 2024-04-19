#ifndef _LEXER_H_
#define _LEXER_H_

#include "base/vasdef.hpp"
#include "symbol.hpp" // see symbol.hpp struct token_t

#include <string>
#include <vector>

#include <folly/Function.h>

namespace vastina {

class lexer {

  public:
    // only local use enum
    enum STATE {
        END = -1,
        NORMAL,
        ERROR
    };
    enum RESULT {
        SUCCESS,
        FAIL
    };

  private:
    std::vector<token_t> tokens;
    std::string buffer;
    u32 offset;
    u32 line;
    u32 lineoffset; // todo
    STATE state;

    Scope::pointer current_scope;

    STATE ParseWhiteSpace();
    RESULT ParseKeyWord(const std::string_view &, TOKEN,
                        const folly::Function<bool(char)> &, TOKEN Default,
                        const folly::Function<bool(char)> &);
    // todo: parse number
    void ParseNumber();
    void forSingelWord(const std::string_view &target, TOKEN target_type);

  public:
    lexer() = default;
    lexer(const char *filename);
    ~lexer();

    i32 Parse();

    STATE Next();
    i32 reScan();  //todo, for non-func func declare

    const std::vector<token_t> &getTokens();
    const std::string_view getBuffer();

    // this is not good
    Scope::pointer getScope();
};

} // namespace vastina

#endif