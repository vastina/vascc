#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "base/Tree.hpp"
#include "expr.hpp"
#include "symbol.hpp"

#include <memory>
#include <vector>

namespace vastina {

using std::make_shared;
using std::shared_ptr;

using TokenPtr = shared_ptr<std::vector<token_t>>;
using ptokens = std::vector<Preprocess::p_token_t>;
using pTokenPtr = shared_ptr<ptokens>;

// saving primary tokens'ref in class private member is better
// typedef struct StmtUnit {
//     pTokenPtr ptks_;
//     u32 pstart_;
//     u32 pend_;
//     StmtUnit(const ptokens &ptks_) : ptks_(make_shared<ptokens>(ptks_)), pstart_(), pend_() {}
//     StmtUnit(const ptokens &ptks_, u32 _start, u32 _end) : ptks_(make_shared<ptokens>(ptks_)), pstart_(_start), pend_(_end) {}
//     StmtUnit(const StmtUnit &other) : ptks_(other.ptks_), pstart_(other.pstart_), pend_(other.pend_) {}
//     StmtUnit(StmtUnit &&other) : ptks_(std::move(other.ptks_)), pstart_(other.pstart_), pend_(other.pend_) {}
//     ~StmtUnit() = default;
// } StmtUnit;

class Stmt {
  public:
    using pointer = Stmt *;

  protected:
    pTokenPtr food_;
    range_t r_;
    Scope::pointer scope_; // the scope it belongs to

  public:
    ~Stmt() = default;
};

template <typename ty>
class FdeclStmt : public Stmt {
  protected:
    Function::pointer func_;

  public:
};

template <typename ty>
class BinStmt : public Stmt {
  protected:
    TreeNode<Expression> root_;

  public:
};

class LoopStmt : public Stmt {
};

}; // namespace vastina

#endif