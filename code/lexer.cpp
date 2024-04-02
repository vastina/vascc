#include "lexer.hpp"

#include "base/String.hpp"
#include "base/vasdef.hpp"

#include <fstream>
#include <iostream>

namespace vastina{

token_t::token_t(TOKEN tk): token(tk) {};
token_t::token_t(TOKEN tk, const std::string& sv): token(tk), data(sv) {};
token_t::token_t(TOKEN tk, std::string&& sv): token(tk), data(sv) {};
token_t::token_t(TOKEN tk, const std::string& sv, unsigned _line): token(tk), data(sv), line(_line) {}
token_t::token_t(TOKEN tk, std::string&& sv, unsigned _line): token(tk), data(sv), line(_line) {}

token_t::token_t(const token_t& tk): token(tk.token), data(tk.data), line(tk.line) {};
token_t::token_t(token_t&& tk): token(tk.token), data(std::move(tk.data)), line(tk.line) {};

//just read file into buffer
lexer::lexer(const char* filename):tokens(), offset(0), line(1), lineoffset(0){
    std::ifstream ifs = std::ifstream();
    ifs.open(filename);
    //if(!ifs.is_open()) throw "can't open file";
    char buf[256];
    while(!ifs.eof()){
        ifs.getline(buf, 256);
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

lexer::RESULT lexer::ParseKeyWord(const std::string& target, TOKEN target_type, bool endjudge(char endsymbol), 
    TOKEN Default, bool DefaultEndjudge(char endsymbol)){
    unsigned len = target.size();
    if(Strcmp(buffer, offset, target)&&endjudge(buffer[offset+len])){
        tokens.push_back(token_t(target_type, target, line));
        offset += len;
        return lexer::RESULT::SUCCESS;
    }
    else if(Default == TOKEN::UNKNOW) return lexer::RESULT::FAIL;
    else{
        std::string temp;
        while (DefaultEndjudge(buffer[offset])){
            temp += buffer[offset];
            offset++;
        }
        tokens.push_back(
            token_t(Default, std::move(temp), line)
        );
        return lexer::RESULT::SUCCESS;
    }
}

void lexer::forSingelWord(const std::string& target, TOKEN target_type){
    tokens.push_back(token_t(target_type, target, line));
            ++offset;
}

void lexer::ParseNumber(){
    
}

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
            ParseKeyWord("asm",     TOKEN::ASM,     [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'b':{
            ParseKeyWord("bool",    TOKEN::BOOL,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'c':{
            ParseKeyWord("char",    TOKEN::BOOL,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'd':{
            ParseKeyWord("double",  TOKEN::BOOL,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'e':{
            ParseKeyWord("else",    TOKEN::ELSE,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'f':{
            RESULT res = 
            ParseKeyWord("for",     TOKEN::FOR,     [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::UNKNOW,  [](char ch){return true;});
            if(res == RESULT::SUCCESS) break;
            else res = 
            ParseKeyWord("float",   TOKEN::FLOAT,   [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::UNKNOW,     [](char ch){return true;});
            if(res == RESULT::SUCCESS) break;
            else res =
            ParseKeyWord("func", TOKEN::FUNC, [](char ch){return (CHARTYPE::OTHER == CharType(ch));}, 
                                    TOKEN::SYMBOL, [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'i':{
            RESULT res = 
            ParseKeyWord("int",     TOKEN::INT,     [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == RESULT::SUCCESS) break;
            else res = 
            ParseKeyWord("if",      TOKEN::IF,      [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'l':{
            ParseKeyWord("let",    TOKEN::LET,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        case 'm':{
            ParseKeyWord("main",    TOKEN::MAIN,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});

            break;
        }
        case 'r':{
            ParseKeyWord("return",  TOKEN::RETURN,  [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});

            break;
        }
        case 'v':{
            ParseKeyWord("var",    TOKEN::VAR,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::SYMBOL,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            break;
        }
        default:
            break;
        }
        break;
    }
    case CHARTYPE::NUM:{
        std::string temp = std::string();
        while (CHARTYPE::NUM == CharType(buffer[offset])){
            temp.push_back(buffer[offset]);
            offset++;
        }
        if(CHARTYPE::OTHER == CharType(buffer[offset])){
            tokens.push_back(
                token_t(TOKEN::NUMBER, std::move(temp), line)
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
            forSingelWord("{", TOKEN::OBRACE);
            break;
        case '}':
            forSingelWord("}", TOKEN::CBRACE);
            break;
        case ';':
            forSingelWord(";", TOKEN::SEMICOLON);
            break;
        case ',':
            forSingelWord(",", TOKEN::COMMA);
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
        case '!':
            forSingelWord("!", TOKEN::LOGNOT);
            break;
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

const std::string& lexer::getBuffer(){
    return buffer;
}

}