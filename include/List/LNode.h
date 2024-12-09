#ifndef LNODE_H
#define LNODE_H

#include <memory>

#include "op.h"

struct LNode {
    using LNode_sptr = std::shared_ptr<LNode>;

    LNode(op oper_ = op()) : oper{oper_}, next{nullptr}, prev{nullptr} {}

    op         oper;
    LNode_sptr next;
    LNode_sptr prev;
};

#endif