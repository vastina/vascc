#include "lexer.hpp"

#include "base/String.hpp"
#include "base/vasdef.hpp"

#include "symbol.hpp" //todo

#include <fstream>
#include <functional>
#include <iostream>
#include <string_view>

namespace vastina {

token_t::token_t(TOKEN tk) : token(tk){};
token_t::token_t(TOKEN tk, const std::string_view &sv)
    : token(tk), name(sv){};
token_t::token_t(TOKEN tk, std::string_view &&sv) : token(tk), name(sv){};
token_t::token_t(TOKEN tk, const std::string_view &sv, unsigned _line)
    : token(tk), name(sv), line(_line) {
}
token_t::token_t(TOKEN tk, std::string_view &&sv, unsigned _line)
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

lexer::RESULT
lexer::ParseKeyWord(const std::string_view &target, TOKEN target_type,
                    const std::function<bool(char)> &endjudge, TOKEN Default,
                    const std::function<bool(char)> &DefaultEndjudge) {
    unsigned len = target.size();
    if (Strcmp(buffer, offset, target) && endjudge(buffer[offset + len])) {
        tokens.push_back(token_t(target_type, target, line));
        offset += len;
        return lexer::RESULT::SUCCESS;
    } else if (Default == TOKEN::UNKNOW)
        return lexer::RESULT::FAIL;
    else {
        unsigned last_offset = offset;
        while (DefaultEndjudge(buffer[offset])) {
            offset++;
        }
        std::string_view temp = {buffer.data() + last_offset, offset - last_offset};
        if (!current_scope->funcExist(temp)) {
            tokens.push_back(token_t(Default, temp, line));
        } else {
            tokens.push_back(token_t(TOKEN::SYMBOLF, temp, line));
        }
        return lexer::RESULT::SUCCESS;
    }
}

void lexer::forSingelWord(const std::string_view &target, TOKEN target_type) {
    tokens.push_back(token_t(target_type, target, line));
    ++offset;
}

void lexer::ParseNumber() {
    // todo
}

constexpr auto SymbolEndJudge = [flag = true](char ch) mutable {
    if (flag) { // 第一个字符不能是数字
        flag = false;
        return (CHARTYPE::CHAR == CharType(ch));
    }
    return (CHARTYPE::OTHER != CharType(ch));
};

constexpr auto NormalEnd = [](char ch) { return (CHARTYPE::OTHER == CharType(ch)); };

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
                                      [](char) { return true; });
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("bool", TOKEN::BOOL, NormalEnd, TOKEN::SYMBOL,
                                   SymbolEndJudge);
            break;
        }
        case 'c': {
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
                                      [](char) { return true; });
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("float", TOKEN::FLOAT, NormalEnd,
                                   TOKEN::UNKNOW, [](char) { return true; });
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("false", TOKEN::FALSE, NormalEnd,
                                   TOKEN::UNKNOW, [](char) { return true; });
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("func", TOKEN::FUNC, NormalEnd,
                                   TOKEN::SYMBOL, SymbolEndJudge);
            if (tokens.at(tokens.size() - 1).token == TOKEN::FUNC) {
                ParseWhiteSpace();
                ParseKeyWord("main", TOKEN::MAIN, NormalEnd, TOKEN::SYMBOLF,
                             SymbolEndJudge);
                current_scope->addFunc(tokens.back().name, Function());
            }
            break;
        }
        case 'g': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'h': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'i': {
            RESULT res = ParseKeyWord("int", TOKEN::INT, NormalEnd, TOKEN::UNKNOW,
                                      [](char) { return false; });
            if (res == RESULT::SUCCESS)
                break;
            else
                res = ParseKeyWord("if", TOKEN::IF, NormalEnd, TOKEN::SYMBOL,
                                   SymbolEndJudge);
            break;
        }
        case 'j': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'k': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'l': {
            RESULT res = ParseKeyWord("long", TOKEN::LONG, NormalEnd, TOKEN::UNKNOW,
                                      [](char) { return false; });
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
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'o': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'p': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'q': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'r': {
            ParseKeyWord("return", TOKEN::RETURN, NormalEnd, TOKEN::SYMBOL,
                         SymbolEndJudge);

            break;
        }
        case 's': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 't': {
            ParseKeyWord("true", TOKEN::TRUE, NormalEnd, TOKEN::UNKNOW,
                         SymbolEndJudge);
            break;
        }
        case 'u': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
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
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'y': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        case 'z': {
            (void)ParseKeyWord(
                "?", TOKEN::UNKNOW, [](char) { return false; },
                TOKEN::SYMBOL, SymbolEndJudge);
            break;
        }
        default:
            break;
        }
        break;
    }
    case CHARTYPE::NUM: {
        unsigned last_offset = offset;
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
            forSingelWord("\"", TOKEN::SQUOTE);
            break;
        case '\'':
            forSingelWord("'", TOKEN::SQUOTE);
            break;
        case '=': {
            RESULT res = ParseKeyWord(
                "==", TOKEN::EQUAL, [](char) { return true; },
                TOKEN::UNKNOW, [](char) { return false; });
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("=", TOKEN::ASSIGN);
            break;
        }
        case '>':
            forSingelWord(">", TOKEN::GREATER);
            break;
        case '<':
            forSingelWord("<", TOKEN::LESS);
            break;
        case '!': {
            RESULT res = ParseKeyWord(
                "!=", TOKEN::NOTEQUAL, [](char) { return true; },
                TOKEN::UNKNOW, [](char) { return false; });
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("!", TOKEN::LOGNOT);
            break;
        }
        case '&': {
            RESULT res = ParseKeyWord(
                "&&", TOKEN::LOGAND, [](char) { return true; },
                TOKEN::UNKNOW, [](char) { return false; });
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("&", TOKEN::AND);
            break;
        }
        case '|': {
            RESULT res = ParseKeyWord(
                "||", TOKEN::LOGOR, [](char) { return true; },
                TOKEN::UNKNOW, [](char) { return false; });
            if (res == RESULT::SUCCESS)
                break;
            else
                forSingelWord("|", TOKEN::OR);
            break;
        }
        case '+':
            forSingelWord("+", TOKEN::ADD);
            break;
        case '-':
            forSingelWord("-", TOKEN::NEG);
            break;
        case '*':
            forSingelWord("*", TOKEN::MULTI);
            break;
        case '/':
            forSingelWord("/", TOKEN::DIV);
            break;
        case '~':
            forSingelWord("~", TOKEN::OPS);
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return STATE::NORMAL;
}

const std::vector<token_t> &
lexer::getTokens() {
    return tokens;
}

const std::string_view
lexer::getBuffer() {
    return {buffer.data(), buffer.size()};
}

Scope::pointer
lexer::getScope() {
    return current_scope;
}

} // namespace vastina