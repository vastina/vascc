#include "symbol.hpp"
#include "base/log.hpp"
#include "base/vasdef.hpp"
#include "expr.hpp"
#include <iostream>

namespace vastina {

#define TEMP_LOG                                                         \
    std::cerr << offset << ' ' << Current() << ' ' << CurrentTokenName() \
              << ' ';
// get the child just created so add data in it
Scope::pointer
Scope::CreateChild(range_t &&rr = {0, 0}) {
    children_.push_back(new Scope(this, std::move(rr)));
    return children_.back();
}
Scope::pointer
Scope::getParent() {
    return parent_;
}

void Scope::addVar(const std::string_view &name, Variable &&var) {
    st_.addVar(name, std::move(var));
}
void Scope::addFunc(const std::string_view &name, Function &&fc) {
    st_.addFunc(name, std::move(fc));
}
Variable::pointer
Scope::getVar(std::string_view name) {
    auto node = this;
    do {
        auto res = node->st_.getVar(name);
        if (nullptr != res)
            return res;
        node = node->parent_;
    } while (nullptr != node);
    return nullptr;
}
Function::pointer
Scope::getFunc(std::string_view name) {
    auto node = this;
    do {
        auto res = node->st_.getFunc(name);
        if (nullptr != res)
            return res;
        node = node->parent_;
    } while (nullptr != node);
    return nullptr;
}
bool Scope::varExist(const std::string_view &name) {
    auto node = this;
    do {
        if (node->st_.varExist(name)) {
            return true;
        }
        node = node->parent_;
    } while (nullptr != node);

    return false;
}

bool Scope::funcExist(const std::string_view &name) {
    auto node = this;
    do {
        if (node->st_.funcExist(name)) {
            return true;
        }
        node = node->parent_;
    } while (nullptr != node);

    return false;
}

void Scope::setRange(unsigned start, unsigned end) {
    r_.start = start;
    r_.end = end;
}
void Scope::setBreakable(bool breakable){
    isBreakable_ = breakable;
}
const range_t &
Scope::getRange() {
    return r_;
}
const decltype(Scope::children_) &Scope::getChildren() { return children_; }
const SymbolTable &
Scope::getSymbolTable() {
    return st_;
}
// you can get other if you like
Scope::pointer
Scope::getNextChild(unsigned offst = 1) {
    idchild += offst;
    return children_.at(idchild - offst);
}
Scope::pointer
Scope::getChildat(unsigned offst) {
    return children_.at(offst);
}

inline TOKEN
Preprocess::Current() {
    return primary_tokens[offset].token;
}

inline const std::string_view &
Preprocess::CurrentTokenName() {
    return primary_tokens[offset].data;
}

inline TOKEN
Preprocess::Peek(unsigned _offset = 1) {
    return primary_tokens[offset + _offset].token;
}

inline TOKEN
Preprocess::PreToken(unsigned _offset = 1) {
    if (_offset > offset)
        return TOKEN::UNKNOW;
    return primary_tokens[offset - _offset].token;
}

void Preprocess::Next() {
    offset++;
}

int Preprocess::Process() {
    unsigned size = primary_tokens.size();
    while (offset < size) {
        switch (Current()) {
        case TOKEN::SEMICOLON:
            Next();
            break;
        case TOKEN::OBRACE: {
            current_scope = current_scope->getNextChild();
            current_scope->setRange(getSize(), 0);
            Next();
            break;
        }
        case TOKEN::CBRACE: {
            current_scope->setRange(current_scope->getRange().start,
                                    getSize());
            current_scope = current_scope->getParent();
            Next();
            break;
        }
        case TOKEN::IF:
            IfType();
            break;
        case TOKEN::SYMBOL: {
            if (!current_scope->varExist(CurrentTokenName())) {
                TEMP_LOG
                EXIT_ERROR
            }
            Except(TOKEN::ASSIGN, false, result);
            if (result == 0) {
                Assign([this]() {
                    return Current() == TOKEN::SEMICOLON || Current() == TOKEN::COMMA;
                });
                break;
            } else
                Except(TOKEN::SEMICOLON, true, result);
            break;
        }
        case TOKEN::SYMBOLF: { // void go here
            if (!current_scope->funcExist(CurrentTokenName())) {
                TEMP_LOG
                EXIT_ERROR
            }
            results.push_back({P_TOKEN::CALL, offset, offset + 1});
            Callee(current_scope->getFunc(CurrentTokenName()));
            break;
        }
        case TOKEN::WHILE:
            LoopW();
            break;
        case TOKEN::FOR:
            LoopF([] { return false; });
            break;
        case TOKEN::DO:
            LoopD([] { return false; });
            break;
        case TOKEN::FUNC:
            FuncDecl();
            break;
        case TOKEN::RETURN:
            RetType();
            break;
        default: {
            switch (token_type(Current())) {
            case TOKEN_TYPE::TYPE: {
                Except(TOKEN::SYMBOL, true, result);
                Declare(
                    [this]() { return Current() == TOKEN::SEMICOLON; });
                break;
            }
            case TOKEN_TYPE::VALUE: {
                tryNext(TOKEN::SEMICOLON, true);
                results.push_back({.tk = P_TOKEN::CAL,
                                   .start = offset - 1,
                                   .end = offset});
                break;
            }

            default:
                std::cerr << Current() << '\n';
                {
                    RETURN_ERROR
                }
            }
        }
        }
    }

    // it should be the global scope now
    current_scope->setRange(0, getSize());
    return 0;
}

// then I will make no check on the Parser of calculate because it is
// checked here
int Preprocess::CalType(const std::function<bool()> &EndJudge) {

    BracketCount bc;
    unsigned last_offset = offset;

    while (true) {
        switch (token_type(Current())) {
        case TOKEN_TYPE::BRAC: {
            if (Current() == TOKEN::NRBRAC) {
                if (++bc.close > bc.open) {
                    TEMP_LOG
                    EXIT_ERROR
                }
            } else
                ++bc.open;
            break;
        }
        case TOKEN_TYPE::VALUE: {
            if (token_type(Peek()) == TOKEN_TYPE::VALUE) {
                RETURN_ERROR
            }

            switch (Current()) {
            case TOKEN::SYMBOL:
                if (!current_scope->varExist(CurrentTokenName())) {
                    TEMP_LOG
                    EXIT_ERROR
                }
                break;
            case TOKEN::SYMBOLF: // nonvoid go here
                if (!current_scope->funcExist(CurrentTokenName())) {
                    TEMP_LOG
                    EXIT_ERROR
                }
                Callee(current_scope->getFunc(CurrentTokenName()));
                break;
            case TOKEN::NUMBER:
                break;
            case TOKEN::STRING:
                break;
            default:
                break;
            }
            break;
        }
        case TOKEN_TYPE::OPERATOR: {
            auto peek = Peek();
            if (token_type(peek) == TOKEN_TYPE::OPERATOR) {
                if (peek != TOKEN::NEG && peek != TOKEN::ADD && peek != TOKEN::OPS && peek != TOKEN::LOGNOT) {
                    {
                        RETURN_ERROR
                    }
                }
            }
            break;
        }
        default: {
            TEMP_LOG;
            RETURN_ERROR
        }
        }
        Next();
        if (EndJudge())
            break;
    }

    results.push_back({P_TOKEN::CAL, last_offset, offset});
    return 0;
}

int Preprocess::Assign(const std::function<bool()> &EndJudge) {

    unsigned last_offset = offset;

    while (true) {
        if (Current() == TOKEN::SYMBOL && Peek() == TOKEN::ASSIGN) {
            Next();
            Next();
        } else {
            results.push_back({P_TOKEN::ASSIGN, last_offset, offset});
            (void)CalType([this]() { return Current() == TOKEN::SEMICOLON; });
            break;
        }
        if (EndJudge())
            break;
    }

    return 0;
}

int Preprocess::Declare(const std::function<bool()> &EndJudge) {

    std::function<void()> adder;
    switch (Current()) {
    // todo: varible need source location to init
    case TOKEN::INT:
        adder = [this]() {
            current_scope->addVar(CurrentTokenName(), variable<int>());
        };
        break;
    case TOKEN::FLOAT:
        adder = [this]() {
            current_scope->addVar(CurrentTokenName(), variable<float>());
        };
        break;
    case TOKEN::DOUBLE:
        break;
    default:
        TEMP_LOG { RETURN_ERROR }
    }
    Next();

    unsigned last_offset = offset;
    while (true) {
        if (Current() == TOKEN::SYMBOL) {
            auto table = current_scope->getSymbolTable();
            if(table.varExist(CurrentTokenName()))
                {EXIT_ERROR}
            adder();
            Next();
        } else if (Current() == TOKEN::ASSIGN) {
            results.push_back({P_TOKEN::ASSIGN, last_offset, offset + 1});
            last_offset = offset + 1;
            Next();
            (void)CalType([this]() {
                return Current() == TOKEN::SEMICOLON || Current() == TOKEN::COMMA;
            });
        } else if (EndJudge())
            break;
        else if (Current() == TOKEN::COMMA) {
            // results.push_back({P_TOKEN::ASSIGN, last_offset,
            // offset});
            last_offset = offset + 1;
            Next();
        }
    }

    return 0;
}

int Preprocess::Address(const std::function<bool()> &EndJudge) {
    return {};
}

int Preprocess::IfType() {
    results.push_back({P_TOKEN::IF, offset, offset + 1});

    tryNext(TOKEN::NLBRAC, true);
    int res = CalType([this]() {
        return Current() == TOKEN::OBRACE;
    }); //暂时不支持不带{}的if
    if (0 != res) {
        RETURN_ERROR
    }

    return 0;
}

int Preprocess::Callee(Function::pointer callee) {
    results.push_back({P_TOKEN::CALL, offset, offset + 1});
    // nothing done here actually
    trySkip(TOKEN::NLBRAC, true);
    if (Current() == TOKEN::NRBRAC)
        return 0;

    while (true) {
        if (CalType([this] {
                return (Current() == TOKEN::COMMA) || (Current() == TOKEN::NRBRAC);
            }) != 0) {
            EXIT_ERROR
        }
        if (Current() == TOKEN::NRBRAC)
            break;
        else
            Next();

        // todo fc
    }

    return 0;
}

int Preprocess::Paras(Function::pointer fc) {
    // just kepp it like this, do not modify if you really know what you
    // are doing
    if (Peek() == TOKEN::NRBRAC) {
        Next();
        return 0;
    } else {
        Next();
    }

    while (true) {
        if (Declare([this] {
                return ((Current() == TOKEN::COMMA) || ((Current() == TOKEN::NRBRAC) && ((Peek() == TOKEN::SEMICOLON) || (Peek() == TOKEN::OBRACE))));
            }) != 0) {
            EXIT_ERROR
        }
        if (Current() != TOKEN::COMMA)
            break;
        else {
            Next();
        }

        // todo fc
    }

    return 0;
}
int Preprocess::FuncDecl() {
    auto last_offset = offset;
    Next();

    auto name = CurrentTokenName();
    if (Current() == TOKEN::MAIN) {
    } // todo
    trySkip(TOKEN::COLON, true);

    std::function<void()> adder;
    switch (Current()) {
    case TOKEN::INT: 
        adder = [this, &name]() { current_scope->addFunc(name, func<int>()); };
        break;
    case TOKEN::FLOAT:
    case TOKEN::CHAR:
    // todo finish this after finish var class
    default:
        THIS_NOT_SUPPORT(CurrentTokenName());
    }
    adder(); // adder or add?

    tryNext(TOKEN::NLBRAC, true);
    current_scope = current_scope->getChildat(id); // getNextchild(0), jump in temply
    (void)Paras(current_scope->getFunc(name));     // no check here
    current_scope = current_scope->getParent();
    tryNext(TOKEN::OBRACE, false);

    if (0 != result)
        tryNext(TOKEN::SEMICOLON, true);

    results.push_back({P_TOKEN::DECL, last_offset, offset});

    if (0 == result)
        return 0;
    {RETURN_ERROR};
}

int Preprocess::RetType() {
    results.push_back({P_TOKEN::RET, offset, offset + 1});
    Next();

    int res = CalType([this] { return Current() == TOKEN::SEMICOLON; });
    if (0 != res) {
        RETURN_ERROR
    }

    return 0;
}

int Preprocess::LoopW() {
    results.push_back({P_TOKEN::LOOP, offset, offset + 1});
    current_scope->getChildat(id)->setBreakable(true);

    tryNext(TOKEN::NLBRAC, true);
    int res = CalType([this]() {
        return Current() == TOKEN::OBRACE;
    }); //暂时不支持不带{}的while
    if (0 != res) {
        RETURN_ERROR
    }

    return 0;
}

int Preprocess::LoopF(const std::function<bool()> &EndJudge) {
    return {};
}

int Preprocess::LoopD(const std::function<bool()> &EndJudge) {
    return {};
}

const Preprocess::p_token_t &
Preprocess::getNext() {
    if (id >= results.size()) {
        {
            EXIT_ERROR
        }
    }
    return static_cast<const p_token_t &>(results[id++]);
}

unsigned
Preprocess::getSize() const {
    return static_cast<unsigned>(results.size());
}

Scope::pointer
Preprocess::CurrentScope() {
    return current_scope;
}
} // namespace vastina
