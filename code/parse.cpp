#include "parse.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "stmt.hpp"
#include "symbol.hpp"

namespace vastina {

inline P_TOKEN
Parser::Current() {
    return processed_tokens_.at(p_offset_).tk;
}

inline const p_token_t &
Parser::CurrentToken() {
    return processed_tokens_.at(p_offset_);
}

inline P_TOKEN
Parser::Peek() {
    return processed_tokens_.at(p_offset_ + 1).tk;
}

inline void
Parser::Next() {
    p_offset_++;
}

inline bool
Parser::isGlobalHere(){
    return scope_->getParent() == nullptr;
}

i32 Parser::Parse() {
    const u32 processed_tokens_size = processed_tokens_.size();
    while (p_offset_ < processed_tokens_size) {
        switch (Current()) {
        case P_TOKEN::BINARY: {

            break;
        }
        case P_TOKEN::FDECL: {

            EXCEPT_ZERO(Fdecl);
            break;
        }
        case P_TOKEN::VDECL: {
            // do{
            //     Except(P_TOKEN::BINARY, false, res_);
            //     if(0 == res_) break;
            //     Except(P_TOKEN::VDECL, false, res_);
            //     if(0 == res_) break;
            //     Except(P_TOKEN::END, false, res_);
            // }while(0);
            EXCEPT_ZERO(Vdecl);
            break;
        }
        case P_TOKEN::ADDR: {

            break;
        }
        case P_TOKEN::IF: {

            break;
        }
        case P_TOKEN::LOOP: {

            break;
        }
        case P_TOKEN::CALL: {
            EXCEPT_ZERO(Call);
            break;
        }
        case P_TOKEN::RET: {

            break;
        }
        case P_TOKEN::END: {

            break;
        }
        default:
            EXIT_ERROR;
        }

        Next();
        if(p_offset_ > scope_->getRange().end){
            //should avoid scope_ fall nullptr here?
            if(scope_->getParent() != nullptr)
                scope_ = scope_->getParent();
        }
        
    }

    return {};
}

i32 Parser::Vdecl(){

    return {};
}

i32 Parser::Fdecl(){
    //todo, if there's only declare, no body
    auto func = scope_->getSymbolTable().getFunc(primary_tokens_.at(CurrentToken().start+1).name);
    //remember back to i32 symbol.cpp:Paras
    auto fstmt = new FdeclStmt(func);
    //find range by start number
    //auto range = scope_->findRange(p_offset_);
    auto res =  fstmt->Parse(primary_tokens_, processed_tokens_, scope_->getNextChild()->getRange());
    if(0 != res) RETURN_ERROR
    else result_.push_back(fstmt);

    return 0;
}

i32 Parser::Call(){

    return {};
}

i32 Parser::Ret(){

    return {};
}

i32 Parser::Ifer(){

    return {};
}

i32 Parser::Loop(){

    return {};
}

i32 Parser::Binary(){

    return {};
}


} // namespace vastina