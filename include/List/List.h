#ifndef LIST_H
#define LIST_H

#include <vector>

#include "List/LNode.h"

using LNode_sptr = LNode::LNode_sptr;

// double linked list
// representation of list: _head -> ... -> _tail
class List {
public:
    List() : _head{nullptr}, _tail{nullptr}, _size{0} {}

    // ELEMENT ACCESS
    op  at(const std::size_t index);
    op& front();
    op& back();

    // CAPACITY
    bool        empty();
    std::size_t size();

    // MODIFIERS
    void clear();
    // void insert();  // i will not implement it due to its useless here
    void push_back(LNode new_node);
    void pop_back();
    void push_front(LNode new_node);
    void pop_front();

    // cut off the entire part starting from the index
    void cut_back(const std::size_t index);

    std::vector<op> to_vector();

private:
    // return node by index
    // LNode_sptr& _at(const std::size_t index);

private:
    LNode_sptr  _head;
    LNode_sptr  _tail;
    std::size_t _size;
};

#endif