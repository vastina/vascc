#include "lexer.hpp"
#include "base/String.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <fstream>

#include <folly/Function.h>

namespace vastina {

token_t::token_t(TOKEN tk) : token(tk){};
token_t::token_t(TOKEN tk, const string_view &sv)
    : token(tk), name(sv){};
token_t::token_t(TOKEN tk, string_view &&sv) : token(tk), name(sv){};
token_t::token_t(TOKEN tk, const string_view &sv, u32 _line)
    : token(tk), name(sv), line(_line) {
}
token_t::token_t(TOKEN tk, string_view &&sv, u32 _line)
    : token(tk), name(sv), line(_line) {
}

token_t::token_t(const token_t &tk)
    : token(tk.token), name(tk.name), line(tk.line){};
token_t::token_t(token_t &&tk)
    : token(tk.token), name(std::move(tk.name)), line(tk.line){};

// just read file into buffer
lexer::lexer(const char *filename)
    : tokens(), offset(0), line(1), lineoffset(0),
      current_scope(new Scope(range_t(0, 0))) {
    std::ifstream ifs = std::ifstream();
    ifs.open(filename);
    // if(!ifs.is_open()) throw "can't open file";
    char buf[2048];
    while (!ifs.eof()) {
        ifs.getline(buf, 2048);
        buffer.append(buf).append("\n");
    }
    ifs.close();
}

lexer::~lexer() { tokens.clear(); };

lexer::STATE
lexer::ParseWhiteSpace() {
    while (isWhiteSpace(buffer[offset])) {
        if (buffer[offset] == '\n')
            line++;
        offset++;
        if (offset == buffer.size() - 1)
            return STATE::END;
    }
    return STATE::NORMAL;
}

void lexer::NextLine() {
    while ('\n' != buffer[offset])
        offset++;
    // offset++; line++;
}

lexer::RESULT
lexer::ParseKeyWord(const string_view &target, TOKEN target_type,
                    const folly::Function<bool(char)> &endjudge, TOKEN Default,
                    const folly::Function<bool(char)> &DefaultEndjudge) {
    u32 len = target.size();
    if (Strcmp(buffer, offset, target) &&
        const_cast<folly::Function<bool(char)> &>(endjudge)(buffer[offset + len])) {
        tokens.push_back(token_t(target_type, target, line));
        offset += len;
        return lexer::RESULT::SUCCESS;
    } else if (Default == TOKEN::UNKNOW)
        return lexer::RESULT::FAIL;
    else {
        u32 last_offset = offset;
        while (const_cast<folly::Function<bool(char)> &>(DefaultEndjudge)(buffer[offset])) {
            offset++;
        }
        string_view temp = {buffer.data() + last_offset, offset - last_offset};
        if (!current_scope->funcExist(temp)) {
            tokens.push_back(token_t(Default, temp, line));
        } else {
            tokens.push_back(token_t(TOKEN::SYMBOLF, temp, line));
        }
        return lexer::RESULT::SUCCESS;
    }
}

void lexer::forSingelWord(const string_view &target, TOKEN target_type) {
    tokens.push_back(token_t(target_type, target, line));
    ++offset;
}

void lexer::ParseNumber() {
}

inline const folly::Function<bool(char)> SymbolEndJudge = [flag{true}](char ch) mutable {
    if (flag) { // 第一个字符不能是数字
        flag = false;
        return (CHARTYPE::CHAR == CharType(ch));
    }
    return (CHARTYPE::OTHER != CharType(ch));
};
inline const folly::Function<bool(char)> NormalEnd = [](char ch) { return (CHARTYPE::OTHER == CharType(ch)); };
inline const folly::Function<bool(char)> Truer = [](char) { return true; };
inline const folly::Function<bool(char)> Falser = [](char) { return false; };

lexer::STATE
lexer::Next() {
    if (offset >= buffer.size())
        return STATE::END;

    state = ParseWhiteSpace();
    if (state == STATE::END)
        return state;

    switch (CharType(buffer[offset])) {
    case CHARTYPE::CHAR: {
        switch (buffer[offset]) {
        case 'a': {
            ParseKeyWord("asm", TOKEN::ASM, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'b': {
            RESULT res = ParseKeyWord("break", TOKEN::BREAK, NormalEnd, TOKEN::UNKNOW,
                                      Truer);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("bool", TOKEN::BOOL, NormalEnd, TOKEN::SYMBOL,
                                   SymbolEndJudge);
            break;
        }
        case 'c': {
            RESULT res = ParseKeyWord("continue", TOKEN::CONTINUE, NormalEnd, TOKEN::UNKNOW,
                                      Truer);
            if (res == RESULT::SUCCESS)
                break;
            else
                ParseKeyWord("char", TOKEN::BOOL, NormalEnd, TOKEN::SYMBOL,
                             SymbolEndJudge);
            break;
        }
        case 'd': {
            ParseKeyWord("double", TOKEN::BOOL, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'e': {
            ParseKeyWord("else", TOKEN::ELSE, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'f': {
            RESULT res = ParseKeyWord("for", TOKEN::FOR, NormalEnd, TOKEN::UNKNOW,
                                      Truer);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("float", TOKEN::FLOAT, NormalEnd,
                                   TOKEN::UNKNOW, Truer);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("false", TOKEN::FALSE, NormalEnd,
                                   TOKEN::UNKNOW, Truer);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("func", TOKEN::FUNC, NormalEnd,
                                   TOKEN::SYMBOL, SymbolEndJudge);
            if (tokens.at(tokens.size() - 1).token == TOKEN::FUNC) {
                ParseWhiteSpace();
                ParseKeyWord("main", TOKEN::MAIN, NormalEnd, TOKEN::SYMBOLF,
                             SymbolEndJudge);
                auto func_token = tokens.back();
                current_scope->addFunc(func_token.name, new Function(func_token));
            }
            break;
        }
        case 'g': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'h': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'i': {
            RESULT res = ParseKeyWord("int", TOKEN::INT, NormalEnd, TOKEN::UNKNOW,
                                      Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("if", TOKEN::IF, NormalEnd, TOKEN::SYMBOL,
                                   SymbolEndJudge);
            break;
        }
        case 'j': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'k': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'l': {
            RESULT res = ParseKeyWord("long", TOKEN::LONG, NormalEnd, TOKEN::UNKNOW,
                                      Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                ParseKeyWord("let", TOKEN::LET, NormalEnd, TOKEN::SYMBOL,
                             SymbolEndJudge);
            break;
        }
        case 'm': {
            ParseKeyWord("main", TOKEN::MAIN, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);

            break;
        }
        case 'n': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'o': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'p': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'q': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'r': {
            ParseKeyWord("return", TOKEN::RETURN, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);

            break;
        }
        case 's': {
            ParseKeyWord("static", TOKEN::STATIC, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 't': {
            ParseKeyWord("true", TOKEN::TRUE, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'u': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'v': {
            ParseKeyWord("var", TOKEN::VAR, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'w': {
            ParseKeyWord("while", TOKEN::WHILE, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);
            break;
        }
        case 'x': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'y': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'z': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, Falser,
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case '_': {
            RESULT res = ParseKeyWord("__LINE__", TOKEN::LINE, NormalEnd, TOKEN::UNKNOW,
                                      Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("__FILE__", TOKEN::FILE, NormalEnd, TOKEN::UNKNOW,
                                   Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                ParseKeyWord("__asm__", TOKEN::ASM, NormalEnd, TOKEN::SYMBOL,
                             SymbolEndJudge);
            break;
        }
        default:
            break;
        }
        break;
    }
    case CHARTYPE::NUM: {
        u32 last_offset = offset;
        while (CHARTYPE::NUM == CharType(buffer[offset])) {
            offset++;
        }
        if (CHARTYPE::OTHER == CharType(buffer[offset])) {
            tokens.push_back(token_t(
                TOKEN::NUMBER,
                {buffer.data() + last_offset, offset - last_offset}, line));
            return STATE::NORMAL;
        } else
            return STATE::ERROR;

        break;
    }
    case CHARTYPE::OTHER: {
        switch (buffer[offset]) {
        case '(':
            forSingelWord("(", TOKEN::NLBRAC);
            break;
        case ')':
            forSingelWord(")", TOKEN::NRBRAC);
            break;
        case '{':
            current_scope = current_scope->CreateChild({0, 0});
            forSingelWord("{", TOKEN::OBRACE);
            break;
        case '}':
            current_scope = current_scope->getParent();
            forSingelWord("}", TOKEN::CBRACE);
            break;
        case ';':
            forSingelWord(";", TOKEN::SEMICOLON);
            break;
        case ',':
            forSingelWord(",", TOKEN::COMMA);
            break;
        case ':':
            forSingelWord(":", TOKEN::COLON);
            break;
        case '"':
            forSingelWord("\"", TOKEN::DQUOTE);
            break;
        case '\'':
            forSingelWord("'", TOKEN::SQUOTE);
            break;
        case '=': {
            RESULT res = ParseKeyWord(
                "==", TOKEN::EQUAL, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "=<", TOKEN::LESSEQUAL, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "=>", TOKEN::GREATEREQUAL, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("=", TOKEN::ASSIGN);
            break;
        }
        case '>': {
            RESULT res = ParseKeyWord(
                ">=", TOKEN::GREATEREQUAL, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    ">>", TOKEN::RSHIFT, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    ">>=", TOKEN::RSHIFTE, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord(">", TOKEN::GREATER);
            break;
        }
        case '<': {
            RESULT res = ParseKeyWord(
                "<=", TOKEN::LESSEQUAL, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "<<", TOKEN::LSHIFT, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "<<=", TOKEN::LSHIFTE, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("<", TOKEN::LESS);
            break;
        }
        case '!': {
            RESULT res = ParseKeyWord(
                "!=", TOKEN::NOTEQUAL, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("!", TOKEN::LOGNOT);
            break;
        }
        case '&': {
            RESULT res = ParseKeyWord(
                "&&", TOKEN::LOGAND, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "&=", TOKEN::ANDE, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("&", TOKEN::AND);
            break;
        }
        case '|': {
            RESULT res = ParseKeyWord(
                "||", TOKEN::LOGOR, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord(
                    "|=", TOKEN::ORE, Truer,
                    TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("|", TOKEN::OR);
            break;
        }
        case '+': {
            RESULT res = ParseKeyWord(
                "+=", TOKEN::SUM, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("+", TOKEN::PLUS);
            break;
        }
        case '-': {
            RESULT res = ParseKeyWord(
                "-=", TOKEN::DIFF, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("-", TOKEN::NEG);
            break;
        }
        case '*': {
            RESULT res = ParseKeyWord(
                "*=", TOKEN::MULTIE, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("*", TOKEN::MULTI);
            break;
        }
        case '/': {
            if (buffer[offset + 1] == '/')
                NextLine(); //
            else {
                RESULT res = ParseKeyWord(
                    "/=", TOKEN::DIVE, Truer,
                    TOKEN::UNKNOW, Falser);
                if (res == RESULT::SUCCESS)
                    break;
                else
                    forSingelWord("/", TOKEN::DIV);
            }
            break;
        }
        case '~':
            forSingelWord("~", TOKEN::OPS);
            break;
        case '^': {
            RESULT res = ParseKeyWord(
                "^=", TOKEN::XORE, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("~", TOKEN::XOR);
            break;
        }
        case '%': {
            RESULT res = ParseKeyWord(
                "%=", TOKEN::MODE, Truer,
                TOKEN::UNKNOW, Falser);
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("%", TOKEN::MOD);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    u32 size = tokens.size();
    if(tokens.back().token == TOKEN::NLBRAC){
        auto &&func_token = tokens.at(size - 2);
        if(func_token.token == TOKEN::SYMBOL && token_type(tokens.at(size - 3).token) == TOKEN_TYPE::TYPE){
            func_token.token = TOKEN::SYMBOLF;
            current_scope->addFunc(func_token.name, new Function(func_token));
        }
    }

    return STATE::NORMAL;
}

i32 lexer::reScan() {
    // for(u32 offst{0u}; auto&& token:tokens){
    //     if(token_type(token.token) == TOKEN_TYPE::TYPE){
    //         if(tokens.at(offst+1).token == TOKEN::SYMBOL && tokens.at(offst+2).token == TOKEN::NLBRAC){
    //             tokens.at(offst+1).token = TOKEN::SYMBOLF;
                
    //         }
    //     }
    //     offst++;
    // }
    return {};
}

i32 lexer::Parse() {
    STATE state;
    while (true) {
        state = this->Next();
        if (STATE::ERROR == state)
            RETURN_ERROR
        else if (STATE::END == state)
            break;
    }

    return reScan();
}

const std::vector<token_t> &
lexer::getTokens() {
    return tokens;
}

const string_view
lexer::getBuffer() {
    return {buffer.data(), buffer.size()};
}

Scope::pointer
lexer::getScope() {
    return current_scope;
}

} // namespace vastina