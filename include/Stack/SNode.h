#ifndef SNODE_H
#define SNODE_H

#include "op.h"

#include <memory>

struct SNode {
    using SNode_sptr = std::shared_ptr<SNode>;
    
    op         oper;
    SNode_sptr next;

    SNode(op oper_ = op(), SNode_sptr next_ = nullptr)
        : oper{oper_}, next{next_}
    {}
};

#endif