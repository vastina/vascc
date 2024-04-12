#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <memory>
#include <vector>

#include "symbol.hpp"

namespace vastina {

using std::make_shared;
using std::shared_ptr;

using TokenPtr = shared_ptr<std::vector<token_t>>;
using ptokens = std::vector<Preprocess::p_token_t>;
using pTokenPtr = shared_ptr<ptokens>;

// saving primary tokens'ref in class private member is better
typedef struct StmtUnit {
    pTokenPtr ptks_;
    unsigned pstart_;
    unsigned pend_;

    StmtUnit(const ptokens &ptks_) : ptks_(make_shared<ptokens>(ptks_)), pstart_(), pend_() {}
    StmtUnit(const ptokens &ptks_, unsigned _start, unsigned _end) : ptks_(make_shared<ptokens>(ptks_)), pstart_(_start), pend_(_end) {}
    StmtUnit(const StmtUnit &other) : ptks_(other.ptks_), pstart_(other.pstart_), pend_(other.pend_) {}
    StmtUnit(StmtUnit &&other) : ptks_(std::move(other.ptks_)), pstart_(other.pstart_), pend_(other.pend_) {}
    ~StmtUnit() = default;
} StmtUnit;

}; // namespace vastina

#endif