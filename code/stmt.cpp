#include "stmt.hpp"

namespace vastina {

void CompoundStmt::addChildren(Stmt::pointer child, i32 pos = -1){
    if(0 > pos) children_.push_back(child);
    else children_.insert(children_.begin() + pos, child);
}


i32 FdeclStmt::Parse(const std::vector<token_t>&, const std::vector<p_token_t>&, range_t) {

    return {};
}

void FdeclStmt::addChildren(Stmt::pointer child){
    body_->addChildren(child);
}

i32 VdeclStmt::Parse(const std::vector<token_t>&, const std::vector<p_token_t>&, range_t) {

    return {};
}

}
