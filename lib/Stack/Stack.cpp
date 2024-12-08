#include "Stack/Stack.h"

#include <stdexcept>

op Stack::top()
{
    if (_length == 0)
        throw std::runtime_error("Trying to get top element of empty stack");

    return _top->oper;
}

bool Stack::empty() { return _length == 0; }

std::size_t Stack::length() { return _length; };

void Stack::push(const op new_oper)
{
    if (_top == nullptr)
        _top = std::make_shared<SNode>(new_oper);
    else {
        SNode_sptr new_node = std::make_shared<SNode>(new_oper);
        new_node->next = _top;

        _top = new_node;
    }

    ++_length;
}

void Stack::pop()
{
    if (_length == 0)
        throw std::runtime_error("Pop from empty stack");
    
    _top = _top->next;

    --_length;
}

void Stack::clear()
{
    while (_top != nullptr)
        pop();
}

std::vector<op> Stack::to_vector()
{
    std::vector<op> result;

    SNode_sptr current = _top;
    while (current) {
        result.emplace_back(current->oper);
        current = current->next;
    }

    return result;
}