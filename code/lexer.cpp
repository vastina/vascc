#include "lexer.hpp"

#include "base/String.hpp"
#include "base/vasdef.hpp"

#include "symbol.hpp"//todo

#include <fstream>
#include <iostream>
#include <functional>

namespace vastina{

token_t::token_t(TOKEN tk): token(tk) {};
token_t::token_t(TOKEN tk, const std::string_view& sv): token(tk), data(sv) {};
token_t::token_t(TOKEN tk, std::string_view&& sv): token(tk), data(sv) {};
token_t::token_t(TOKEN tk, const std::string_view& sv, unsigned _line): token(tk), data(sv), line(_line) {}
token_t::token_t(TOKEN tk, std::string_view&& sv, unsigned _line): token(tk), data(sv), line(_line) {}

token_t::token_t(const token_t& tk): token(tk.token), data(tk.data), line(tk.line) {};
token_t::token_t(token_t&& tk): token(tk.token), data(std::move(tk.data)), line(tk.line) {};

//just read file into buffer
lexer::lexer(const char* filename):tokens(), offset(0), line(1), lineoffset(0), current_scope(new Scope(range_t(0,0))){
    std::ifstream ifs = std::ifstream();
    ifs.open(filename);
    //if(!ifs.is_open()) throw "can't open file";
    char buf[2048];
    while(!ifs.eof()){
        ifs.getline(buf, 2048);
        buffer.append(buf).append("\n");
    }
std::cout << buffer <<'\n';
    ifs.close();
}

lexer::~lexer(){tokens.clear();};

lexer::STATE lexer::ParseWhiteSpace(){
    while (isWhiteSpace(buffer[offset]))
    {
        if(buffer[offset]=='\n') line++;
        offset++;
        if(offset == buffer.size()-1) return STATE::END;
    }
    return STATE::NORMAL;
}

lexer::RESULT lexer::ParseKeyWord(const std::string_view& target, TOKEN target_type, std::function<bool(char)> endjudge, 
    TOKEN Default, std::function<bool(char)> DefaultEndjudge){
    unsigned len = target.size();
    if(Strcmp(buffer, offset, target)&&endjudge(buffer[offset+len])){
        tokens.push_back(token_t(target_type, target, line));
        offset += len;
        return lexer::RESULT::SUCCESS;
    }
    else if(Default == TOKEN::UNKNOW) return lexer::RESULT::FAIL;
    else{
        //std::string temp;
        unsigned last_offset = offset;
        while (DefaultEndjudge(buffer[offset])){
            //temp += buffer[offset];
            offset++;
        }
//bad
if(!current_scope->funcExist({buffer.data()+last_offset, offset-last_offset}))
        tokens.push_back(
            token_t(Default, {buffer.data()+last_offset, offset-last_offset}, line)
        );
        return lexer::RESULT::SUCCESS;
    }
}

void lexer::forSingelWord(const std::string_view& target, TOKEN target_type){
    tokens.push_back(token_t(target_type, target, line));
            ++offset;
}

void lexer::ParseNumber(){
    
}

constexpr auto SymbolEndJudge = [flag=true](char ch)mutable{
    if(flag){//第一个字符不能是数字
        flag = false;
        return (CHARTYPE::CHAR == CharType(ch));
    }
    return (CHARTYPE::OTHER != CharType(ch));
};

constexpr auto NormalEnd = [](char ch){
    return (CHARTYPE::OTHER == CharType(ch));
};

lexer::STATE lexer::Next(){
    if(offset >= buffer.size()) return STATE::END;

    state = ParseWhiteSpace();
    if(state == STATE::END) return state;

    switch (CharType(buffer[offset]))
    {
    case CHARTYPE::CHAR:{
        switch (buffer[offset])
        {
        case 'a':{
            ParseKeyWord("asm",     TOKEN::ASM,     NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'b':{
            ParseKeyWord("bool",    TOKEN::BOOL,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'c':{
            ParseKeyWord("char",    TOKEN::BOOL,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'd':{
            ParseKeyWord("double",  TOKEN::BOOL,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'e':{
            ParseKeyWord("else",    TOKEN::ELSE,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'f':{
            RESULT res = 
            ParseKeyWord("for",     TOKEN::FOR,     NormalEnd,
                                    TOKEN::UNKNOW,      [](char ch){return true;});
            if(res == RESULT::SUCCESS) break;
            else res = 
            ParseKeyWord("float",   TOKEN::FLOAT,   NormalEnd,
                                    TOKEN::UNKNOW,      [](char ch){return true;});
            if(res == RESULT::SUCCESS) break;
            else res =
            ParseKeyWord("func",    TOKEN::FUNC,    NormalEnd, 
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            if(tokens.at(tokens.size()-1).token == TOKEN::FUNC){
                ParseWhiteSpace();
            ParseKeyWord("main",    TOKEN::MAIN,    NormalEnd, 
                                    TOKEN::SYMBOLF,     SymbolEndJudge);
            current_scope->addFunc(tokens.back().data, Function());
            }
            break;
        }
        case 'g':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'h':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'i':{
            RESULT res = 
            ParseKeyWord("int",     TOKEN::INT,     NormalEnd,
                                    TOKEN::UNKNOW,      [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
            else res = 
            ParseKeyWord("if",      TOKEN::IF,      NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'j':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'k':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'l':{
            ParseKeyWord("let",     TOKEN::LET,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'm':{
            ParseKeyWord("main",    TOKEN::MAIN,    NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);

            break;
        }
        case 'n':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'o':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'p':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'q':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'r':{
            ParseKeyWord("return",  TOKEN::RETURN,  NormalEnd,
                                    TOKEN::SYMBOL,     SymbolEndJudge);

            break;
        }
        case 's':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 't':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'u':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'v':{
            ParseKeyWord("var",    TOKEN::VAR,    NormalEnd,
                                   TOKEN::SYMBOL,     SymbolEndJudge);
            break;
        }
        case 'w':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'x':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'y':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        case 'z':{
            (void)
            ParseKeyWord("?",       TOKEN::UNKNOW,  [](char ch){return false;},
                                    TOKEN::SYMBOL,      SymbolEndJudge);
            break;
        }
        default:
            break;
        }
        break;
    }
    case CHARTYPE::NUM:{
        unsigned last_offset = offset;
        while (CHARTYPE::NUM == CharType(buffer[offset])){
            offset++;
        }
        if(CHARTYPE::OTHER == CharType(buffer[offset])){
            tokens.push_back(
                token_t(TOKEN::NUMBER, {buffer.data()+last_offset, offset-last_offset}, line)
            );
            return STATE::NORMAL;
        }
        else return STATE::ERROR;
        
        break;
    }
    case CHARTYPE::OTHER:{
        switch (buffer[offset])
        {
        case '(':
            forSingelWord("(", TOKEN::NLBRAC);
            break;
        case ')':
            forSingelWord(")", TOKEN::NRBRAC);
            break;
        case '{':
            current_scope = current_scope->CreateChild({0,0});
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
        case '=':{
            RESULT res = 
            ParseKeyWord("==",      TOKEN::EQUAL,   [](char ch){return true;},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
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
        case '!':{
            RESULT res = 
            ParseKeyWord("!=",      TOKEN::NOTEQUAL,[](char ch){return true;},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
            else
                forSingelWord("!", TOKEN::LOGNOT);
            break;
        }
        case '&':{
            RESULT res = 
            ParseKeyWord("&&",      TOKEN::LOGAND,  [](char ch){return true;},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
            else
                forSingelWord("&", TOKEN::AND);
            break;
        }   
        case '|':{
            RESULT res = 
            ParseKeyWord("||",      TOKEN::LOGOR,   [](char ch){return true;},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
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

const std::vector<token_t>& lexer::getTokens(){
    return tokens;
}

const std::string_view lexer::getBuffer(){
    return {buffer.data(), buffer.size()};
}

Scope::pointer lexer::getScope(){
    return current_scope;
}

}