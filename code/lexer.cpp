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

STATE lexer::ParseWhiteSpace(){
    while (isWhiteSpace(buffer[offset]))
    {
        offset++;
        if(offset == buffer.size()-1) return STATE::END;
    }
    return STATE::NORMAL;
}

void lexer::whatname(const std::string& target, TOKEN target_type, bool endjudge(char endsymbol), 
    TOKEN Default, bool DefaultEndjudge(char endsymbol)){
    unsigned len = target.size();
    if(Strcmp(buffer, offset, target)&&endjudge(buffer[offset+len])){
        tokens.push_back(std::move(token_t(target_type, target)));
        offset += len;
    }
    else if(Default == UNKNOW) return;
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
    }
}

STATE lexer::Next(){
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
            whatname("int", TOKEN::INT, [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                            TOKEN::VAR, [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            // if(Strcmp(buffer, offset, "int")
            //     && (CHARTYPE::OTHER == CharType(buffer[offset+3])) ){
            //     tokens.push_back(std::move(token_t(TOKEN::INT, "int")));
            //     offset += 3;
            // }
            // else{
            //     std::string temp = "i";
            //     while (CHARTYPE::OTHER != CharType(buffer[++offset])){
            //         temp += buffer[offset];
            //     }
            //     tokens.push_back(
            //         std::move(token_t(TOKEN::VAR, 
            //         std::move(temp)))
            //     );
            // }
            break;
        }// these three need to be more generalized
        case 'm':{
            whatname("main",TOKEN::MAIN, [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                            TOKEN::VAR,  [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            // if(Strcmp(buffer, offset, "main")
            //     && (CHARTYPE::OTHER == CharType(buffer[offset+4])) ){
            //     tokens.push_back(std::move(token_t(TOKEN::MAIN, "main")));
            //     offset += 4;
            // }
            // else{
            //     std::string temp = "m";
            //     while (CHARTYPE::OTHER != CharType(buffer[++offset])){
            //         temp += buffer[offset];
            //     }
            //     tokens.push_back(
            //         std::move(token_t(TOKEN::VAR, 
            //         std::move(temp)))
            //     );
            // }
            break;
        }
        case 'r':{
            whatname("main",TOKEN::RETURN,  [](char ch){return (CHARTYPE::OTHER == CharType(ch));},
                            TOKEN::VAR,     [](char ch){return (CHARTYPE::OTHER != CharType(ch));});
            // if(Strcmp(buffer, offset, "return")
            //     && (CHARTYPE::OTHER == CharType(buffer[offset+6])) ){
            //     tokens.push_back(std::move(token_t(TOKEN::RETURN, "return")));
            //     offset += 6;
            // }
            // else{
            //     std::string temp = "r";
            //     while (CHARTYPE::OTHER != CharType(buffer[++offset])){
            //         temp += buffer[offset];
            //     }
            //     tokens.push_back(
            //         std::move(token_t(TOKEN::VAR, 
            //         std::move(temp)))
            //     );
            // }
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
            tokens.push_back(std::move(token_t(NLBRAC, "(")));
            ++offset;
            break;
        case ')':
            tokens.push_back(std::move(token_t(NRBRAC, ")")));
            ++offset;
            break;
        case '{':
            tokens.push_back(std::move(token_t(OBRACE, "{")));
            ++offset;
            break;
        case '}':
            tokens.push_back(std::move(token_t(CBRACE, "}")));
            ++offset;
            break;
        case ';':
            tokens.push_back(std::move(token_t(SEMICOLON, ";")));
            ++offset;
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

const std::vector<token_t>* lexer::getTokens(){
    return &tokens;
}

}