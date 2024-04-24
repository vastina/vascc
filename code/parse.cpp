#include "parse.hpp"

#include "base/Tree.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"

#include <queue>

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

inline const p_token_t &
Parser::PeekToken() {
    return processed_tokens_.at(p_offset_ + 1);
}

inline const p_token_t &
Parser::Peekat(u32 offset) {
    return processed_tokens_.at(offset);
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
            EXCEPT_ZERO(Binary);
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

            // when will this be done................................................................
            break;
        }
        case P_TOKEN::IF: {
            EXCEPT_ZERO(Ifer);
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
            EXCEPT_ZERO(Ret);
            break;
        }
        case P_TOKEN::END: {
            break;
        }
        default:
            EXIT_ERROR;
        }

        Next();
        // print("\np_offset: {}, end{}\n", p_offset_, scope_->getRange().end);
        if (p_offset_ >= scope_->getRange().end) {
            // should avoid fall nullptr here?
            if (scope_->getParent() != nullptr)
                scope_ = scope_->getParent();
            if (current_stmt_->getParent() != nullptr)
                current_stmt_ = current_stmt_->getParent();
        }
    }

    while (scope_->getParent() != nullptr)
        scope_ = scope_->getParent();
    scope_->reSet();

    return 0;
}

i32 Parser::Vdecl() {
    auto var = scope_->getVar(primary_tokens_.at(CurrentToken().start).name);
    auto vstmt = new VdeclStmt(current_stmt_, var);

    current_stmt_->addChildren(vstmt);

    if (Peek() == P_TOKEN::BINARY) { // if with literal or sth to init
        Next();
        vstmt->InitWithStmt(Binary({CurrentToken().start, CurrentToken().end}));
    }

    return 0;
}

i32 Parser::Fdecl() {
    // todo, if there's only declare, no body
    // and this is so stupid
    auto func = scope_->getFunc(primary_tokens_.at(CurrentToken().start + 1).name);
    auto fstmt = new FdeclStmt(func, current_stmt_);
    current_stmt_->addChildren(fstmt);
    current_stmt_ = fstmt;
    scope_ = scope_->getNextChild();
    /*...*/

    return 0;
}

i32 Parser::Call() {

    auto callee = Callee(p_offset_);
    auto callstmt = new CallStmt(current_stmt_, callee);
    current_stmt_->addChildren(callstmt);

    return 0;
}

i32 Parser::Ret() {

    Next();
    auto ret = new RetStmt(current_stmt_, Binary({CurrentToken().start, CurrentToken().end}));
    current_stmt_->addChildren(ret);

    return 0;
}

i32 Parser::Ifer() {

    Next();
    auto ifstmt = new IfStmt(current_stmt_, Binary({CurrentToken().start, CurrentToken().end}));
    current_stmt_->addChildren(ifstmt);
    current_stmt_ = ifstmt;
    scope_ = scope_->getNextChild();

    return 0;
}

i32 Parser::Loop() {

    Next();
    auto lstmt = new LoopStmt(current_stmt_, Binary({CurrentToken().start, CurrentToken().end}));
    current_stmt_->addChildren(lstmt);
    current_stmt_ = lstmt;
    // print("\npppppppp_offset: {}, start:{}, end{}\n", p_offset_, scope_->getRange().start ,scope_->getRange().end);
    scope_ = scope_->getNextChild();

    return 0;
}

i32 Parser::Binary() {
    current_stmt_->addChildren(Binary({CurrentToken().start, CurrentToken().end}));

    return 0;
}

// pos: p_token func name lies
CallExpr::pointer Parser::Callee(u32 pos) {
    auto &token = primary_tokens_.at(Peekat(pos).start);
    auto func = scope_->getFunc(token.name);
    auto callexpr = new class CallExpr(func, token);
    for (auto i{1u}; i <= func->getParamSize(); i++) {
        auto start{Peekat(pos + i).start};
        callexpr->addPara(ParseBinary(start, Peekat(pos + i).end));
        Next();
    }
    return callexpr;
}

BinStmt::pointer
Parser::Binary(range_t r) {
    auto bstmt = new BinStmt(current_stmt_, scope_);
    auto start = r.start;
    auto root{ParseBinary(start, r.end)};
    bstmt->setRoot(root);
    // print("poffset: {}\n", p_offset_);
    // root->Walk(PREORDER, [](const Expression::pointer &_data) { print("token : {}\n", _data->getName()); });
    // putchar('\n');
    return bstmt;
}

typename TreeNode<Expression::pointer>::pointer
Parser::ParseBinary(u32 &offset, u32 end) {
    auto root = BinStmt::nodeCreator(primary_tokens_.at(offset), scope_);
    if (offset + 1 >= end)
        return root;

    u32 last_offset = offset;
    while (true) {
        auto current = BinStmt::nodeCreator(primary_tokens_.at(offset), scope_);
        offset++;
        auto tk = current->data->getToken();
        switch (token_type(tk)) {
        case TOKEN_TYPE::BRAC: {
            if (TOKEN::NRBRAC == tk) {
                root->data->setLevel(Level(TOKEN::SYMBOL));
                return root;
            } else {
                if (TOKEN_TYPE::BRAC == token_type(root->data->getToken())) {
                    root = ParseBinary(offset, end);
                } else {
                    current = ParseBinary(offset, end);
                    goto I_DONOT_LIKE_THIS;
                }
            }
            break;
        }
        I_DONOT_LIKE_THIS:
        case TOKEN_TYPE::VALUE: {
            if (current->data->getToken() == TOKEN::SYMBOLF) {
                offset += Peekat(p_offset_).end - Peekat(p_offset_).start + 1 +1;//skip ')', the last char of fun call
                Next();
                current->data = Callee(p_offset_ );
            }
            if (token_type(root->data->getToken()) == TOKEN_TYPE::VALUE) {
                root = current;
                break;
            }
            auto temp = root->FindChildR(
                [](const typename TreeNode<Expression::pointer>::pointer _node) { return nullptr == _node->right; });
            temp->InsertRight(current);
            break;
        }
        case TOKEN_TYPE::OPERATOR: {
            if(last_offset == offset) break;
            if (current->data->getLevel() >= root->data->getLevel()) {
                root->ReplaceByL(current);
                root = current;
            } else {
                auto temp = root->FindChildR(
                    [&current](const typename TreeNode<Expression::pointer>::pointer _node) {
                        return (nullptr == _node->right) || (_node->data->getLevel() <= current->data->getLevel());
                    });
                if (temp != root)
                    temp->ReplaceByL(current);
                else
                    temp->InsertRight(current);
            }
            break;
        }

        default:
            return nullptr;
        }

        if (offset >= end)
            break;
    }
    // root->Walk(PREORDER, [](const Expression::pointer &_data) { print("token : {}\n", _data->getName()); });
    // putchar('\n');
    return root;
}

void Parser::Walk() {
    std::queue<Stmt::pointer> que;
    std::queue<u32> quee;
    que.push(current_stmt_);
    quee.push(1);
    while (!que.empty()) {
        auto stmt = que.front();
        que.pop();
        auto level = quee.front();
        quee.pop();
        print("level:{}, name:{}\n", level, stmt->getName());
        for (auto &&child : stmt->getChildren()) {
            que.push(child);
            quee.push(level + 1);
        }
    }
}

} // namespace vastina