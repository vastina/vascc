#include "lexer.hpp"

#include "base/String.hpp"

#include <fstream>

namespace vastina{

token_t::token_t(TOKEN tk): token(tk) {};

token_t::token_t(TOKEN tk, const std::string& sv): token(tk), data(sv) {};
token_t::token_t(TOKEN tk, std::string&& sv): token(tk), data(sv) {};



//just read file into buffer
lexer::lexer(const char* filename):tokens(), offset(0) {
    std::ifstream ifs = std::ifstream();
    ifs.open(filename);
    if(!ifs.is_open()) throw "can't open file";
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
        offset++;
        if(offset == buffer.size()-1) return STATE::END;
    }
    return STATE::NORMAL;
}

lexer::RESULT lexer::ParseKeyWord(const std::string& target, TOKEN target_type, bool endjudge(char endsymbol), 
    TOKEN Default, bool DefaultEndjudge(char endsymbol)){
    unsigned len = target.size();
    if(Strcmp(buffer, offset, target)&&endjudge(buffer[offset+len])){
        tokens.push_back(std::move(token_t(target_type, target)));
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
            std::move(token_t(Default, 
            std::move(temp)))
        );
        return lexer::RESULT::SUCCESS;
    }
}

void lexer::forSingelWord(const std::string& target, TOKEN target_type){
    tokens.push_back(std::move(token_t(target_type, target)));
            ++offset;
}

void lexer::ParseNumber(){
    
}

lexer::STATE lexer::Next(){
    if(offset > buffer.size()) return STATE::END;

    state = ParseWhiteSpace();
    if(state == STATE::END) return state;


    CHARTYPE chartype = CharType(buffer[offset]);
    switch (chartype)
    {
    case CHARTYPE::CHAR:{
        switch (buffer[offset])
        {
        case 'i':{
            int res = 
            ParseKeyWord("int",     TOKEN::INT,     [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::UNKNOW,  [](char ch){return false;});
            if(res == 0) break;
            else
            ParseKeyWord("if",      TOKEN::IF,      [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::VAR,     [](char ch){return (CHARTYPE::OTHER == CharType(ch));});
            break;
        }// these three need to be more generalized
        case 'm':{
            ParseKeyWord("main",    TOKEN::MAIN,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::VAR,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});

            break;
        }
        case 'r':{
            ParseKeyWord("return",  TOKEN::RETURN,  [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::VAR,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});

            break;
        }
        case 'e':{
            ParseKeyWord("else",    TOKEN::ELSE,    [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                                    TOKEN::VAR,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
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
        if(buffer[offset]==';'||buffer[offset]==')'||isWhiteSpace(buffer[offset])){
            tokens.push_back(
                std::move(token_t(TOKEN::NUMBER, 
                std::move(temp)))
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
        case '=':
            forSingelWord("=", TOKEN::ASSIGN);
            break;
        case '>':
            forSingelWord(">", TOKEN::GREATER);
            break;
        case '<':
            forSingelWord("<", TOKEN::LESS);
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