#ifndef STACK_H
#define STACK_H

#include "Stack/SNode.h"

#include <vector>
#include <memory>

class Stack {
public:
    using SNode_sptr = SNode::SNode_sptr;

    // Constructor
    Stack() : _top{nullptr}, _length{0} {}

    // GETTERS
    op          top();
    bool        empty();
    std::size_t length();

    // MODIFIERS
    void push(const op new_oper);
    void pop();
    void clear();

    std::vector<op> to_vector();
    
    ~Stack() { this->clear(); }

private:
    SNode_sptr  _top;
    std::size_t _length;
};

#endif