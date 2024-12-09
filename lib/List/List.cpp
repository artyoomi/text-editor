#include "List/List.h"

#include <stdexcept>

op List::at(const std::size_t index)
{
    // LNode_sptr& node = _at(index);
    // return node->oper;

    if (index >= _size)
        throw std::out_of_range("Invalid index in list");

    std::size_t tmp_index = 0;

    LNode_sptr current = _head;
    while (current) {
        if (tmp_index == index)
            return current->oper;

        ++tmp_index;
        current = current->next;
    }
}

op& List::front()
{
    if (_size == 0)
        throw std::runtime_error("Trying to take front element from empty list");
    
    return _head->oper;
}

op& List::back()
{
    if (_size == 0)
        throw std::runtime_error("Trying to take back element from empty list");
    
    return _tail->oper;
}

bool List::empty() { return _size == 0; }

std::size_t List::size() { return _size; }

void List::clear()
{
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}

void List::push_back(LNode new_node)
{
    // zero elements in list
    if (_head == nullptr) {
        _head = std::make_shared<LNode>(new_node);
        _tail = _head;
    // at least one element in list
    } else {
        // _tail  -> new_node
        _tail->next = std::make_shared<LNode>(new_node);
        // _tail <-> new_node
        _tail->next->prev = _tail;
        _tail = _tail->next;
    }

    ++_size;
}

void List::pop_back()
{
    if (_size == 0)
        throw std::runtime_error("Can't pop_back from empty list");
    
    if (_size == 1) {
        _head = nullptr;
        _tail = nullptr;
    } else {
        // prev <-> _tail => prev <- _tail
        _tail->prev->next = nullptr;
        // _tail = prev
        _tail = _tail->prev;
    }

    --_size;
}

void List::push_front(LNode new_node)
{
    // if we have 0 elements in list, push_back equal to push_front
    if (_head == nullptr) {
        this->push_back(new_node);
    // if we have at least 1 element in list
    } else {
        // new_node <-  _head
        _head->prev = std::make_shared<LNode>(new_node);
        // new_node <-> _head
        _head->prev->next = _head;
        // _head = new_node
        _head = _head->prev;

        ++_size;
    }
}

void List::pop_front()
{
    if (_size <= 1) {
        this->pop_back();
    } else {
        // _head -> node
        _head->next->prev = nullptr;
        // _head = node
        _head = _head->next;

        --_size;
    }
}

// i do this double implementation (with at function) due to
// strange error with shared_ptr
void List::cut_back(const std::size_t index)
{
    if (index >= _size)
        throw std::out_of_range("Invalid index to cut_back");

    LNode_sptr part_to_cut = _head;
    std::size_t tmp_index = 0;
    while(part_to_cut) {
        if (tmp_index == index) {
            if (part_to_cut == _head) {
                _head = nullptr;
                _tail = nullptr;
            } else {
                _tail = part_to_cut->prev;
                _tail->next = nullptr;
            }

            _size -= (_size - index);
            return;
        }

        ++tmp_index;
        part_to_cut = part_to_cut->next;
    }
}

std::vector<op> List::to_vector()
{
    std::vector<op> result;

    LNode_sptr current = _head;
    while (current) {
        result.push_back(current->oper);

        current = current->next;
    }

    return result;
}

// LNode_sptr& List::_at(const std::size_t index)
// {
//     if (index >= _size)
//         throw std::out_of_range("Invalid index in list");

//     std::size_t tmp_index = 0;

//     LNode_sptr current = _head;
//     while (current) {
//         if (tmp_index == index)
//             return current;

//         ++tmp_index;
//         current = current->next;
//     }
// }