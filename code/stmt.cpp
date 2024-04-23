#include "stmt.hpp"
#include "base/Tree.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "expr.hpp"
#include "symbol.hpp"

namespace vastina {

void CompoundStmt::addChildren(Stmt::pointer child, i32 pos = -1){
    if(0 > pos) children_.push_back(child);
    else children_.insert(children_.begin() + pos, child);
}

void CondStmt::setCondition(Stmt::pointer condtion){condition_ = dynamic_cast<BinStmt::pointer>(condtion);}

Expression::pointer BinStmt::Creator(const token_t& token, const Scope::pointer scope){
    auto tk = token.token;
    switch (token_type(tk)) {
        case TOKEN_TYPE::TYPE:
            THIS_NOT_SUPPORT("type cast");
        case TOKEN_TYPE::BRAC:
        case TOKEN_TYPE::OPERATOR:
            return new OpExpr(token);
        case TOKEN_TYPE::VALUE:
            if(TOKEN::SYMBOL == tk){
                if(!scope->getSymbolTable().varExist(token.name)){
                    LEAVE_MSG("var not exist");
                    return nullptr;
                } else{
                    return new ValExpr(scope->getVar(token.name));
                }
            } else {
                return new ValExpr(token);
            }
        default:
            LEAVE_MSG("no match token_type");
            return nullptr;
    }
}

typename  TreeNode<Expression::pointer>::pointer
BinStmt::nodeCreator(const token_t& tk){
    auto data = Creator(tk, scope_) ;
    data->setLevel(tk.token);
    return new TreeNode<Expression::pointer>(data);
}

typename  TreeNode<Expression::pointer>::pointer
BinStmt::doParse(const std::vector<token_t> &food, u32 end, u32 &offset){

    auto root = nodeCreator(food.at(offset));
    if(offset >= end) return root;

    while(true){
        auto current = nodeCreator(food.at(offset) );
        offset++;
        auto tk = current->data->getToken();
        switch (token_type(tk)) {
            case TOKEN_TYPE::BRAC:{
                if(TOKEN::NRBRAC == tk){
                    root->data->setLevel(0);
                    return root;
                } else {
                    if(TOKEN_TYPE::BRAC == token_type(root->data->getToken())) root = doParse(food, end, offset);
                    else current = doParse(food, end, offset);
                }
                break;
            }
            case TOKEN_TYPE::VALUE:{
                if(token_type(root->data->getToken()) == TOKEN_TYPE::VALUE) break;
                auto temp = root->FindChildR(
                    [](const typename TreeNode<Expression::pointer>::pointer _node){return nullptr == _node->right;}
                );
                temp->InsertRight(current);
                break;
            }
            case TOKEN_TYPE::OPERATOR:{
                if(current->data->getLevel() >= root->data->getLevel()){
                    root->ReplaceByL(current);
                    root = current;
                } else {
                    auto temp = root->FindChildR(
                        [&current](const typename TreeNode<Expression::pointer>::pointer _node){
                            return (nullptr == _node->right) || (_node->data->getLevel() <= current->data->getLevel());
                    });
                    if(temp != root) temp->ReplaceByL(current);
                    else temp->InsertRight(current);
                }
                break;
            }

            default:
                return nullptr;
        }

        if(offset >= end) break;
    }

    return root;
}

void BinStmt::Parse(const std::vector<token_t> &primary_tokens, range_t r){
    auto start = r.start;
    auto end = r.end;
    root_ = doParse(primary_tokens, end, start);
//for test
    root_->Walk(PREORDER,  
        [](const Expression::pointer& _data){ print("token : {}\n", _data->getName()); });
}

}
