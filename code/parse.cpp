#include "parse.hpp"

#include "base/log.hpp"
#include "stmt.hpp"

namespace vastina {

#define LOG_ERROR print(""); //so todo

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

inline const p_token_t &
Parser::PeekToken(){
    return processed_tokens_.at(p_offset_ + 1);
}

inline void
Parser::Next() {
    p_offset_++;
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
            EXCEPT_ZERO(Loop);
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
        if(p_offset_ >= scope_->getRange().end){
            //should avoid fall nullptr here?
            if(scope_->getParent() != nullptr)
                scope_ = scope_->getParent();
            if(current_stmt_->getParent() != nullptr)
                current_stmt_ = current_stmt_->getParent();
        }
        
    }

    return {};
}

// I know using new directly is bad

i32 Parser::Vdecl(){
    auto var = scope_->getSymbolTable().getVar(primary_tokens_.at(CurrentToken().start).name);
    auto vstmt = new VdeclStmt(current_stmt_, var);

    current_stmt_->addChildren(vstmt);

    if(Peek() == P_TOKEN::BINARY){ //if with literal or sth to init
        auto bstmt = new BinStmt(current_stmt_, scope_);
        Next();
        bstmt->Parse(primary_tokens_, {CurrentToken().start, CurrentToken().end});
        vstmt->InitWithStmt(bstmt);
    }

    return 0;
}

i32 Parser::Fdecl(){
    //todo, if there's only declare, no body
    //and this is so stupid
    auto func = scope_->getSymbolTable().getFunc(primary_tokens_.at(CurrentToken().start+1).name);
    //remember back to i32 symbol.cpp:Paras
    auto fstmt = new FdeclStmt(func, current_stmt_);
    current_stmt_->addChildren(fstmt);
    current_stmt_ = fstmt;
    
    scope_ = scope_->getNextChild();
    /*...*/
    
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

    auto lstmt = new LoopStmt(current_stmt_);
    current_stmt_ = lstmt;

    auto bstmt = new BinStmt(current_stmt_);
    /*bstmt do sth*/
    current_stmt_->setCondition(bstmt);

    return 0;
}

i32 Parser::Binary(){

    return {};
}

void Parser::Walk(){
    print("at top is {}\n", current_stmt_->getName());

//......
    auto fuck = dynamic_cast<CompoundStmt::pointer>(current_stmt_);
    for(auto i{0u}; i<fuck->getStmtSize(); i++){
        auto child = fuck->getChildat(i);
        print("{}, {}\n", i, child->getName());
    }
}

} // namespace vastina