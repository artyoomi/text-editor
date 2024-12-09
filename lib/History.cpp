#include "History.h"

#include <iostream>

const std::size_t History::hist_size = 25;

List History::get_list() { return _ops; }

int  History::get_current_index() { return _current_index; }

void History::add(op oper, Rope &rope)
{
    // if we undo all changes
    if (_current_index == -1)
        _ops.clear();
    // if we not at end of list -> cut entire right part
    else if (_ops.size() != 0 && _current_index < _ops.size() - 1)
        _ops.cut_back(_current_index + 1);

    // add new operation at end of deque
    _ops.push_back(oper);

    // change rope with given oper
    execute(oper, rope);

    // check for change history size
    if (_ops.size() > hist_size) {
        _ops.pop_front();
        --_current_index;
    }
    
    ++_current_index;
}

void History::execute(op oper, Rope &rope)
{
    switch (oper.type) {
    case op_type::INSERT:
        rope.insert(oper.beg, oper.substr);
        break;
    case op_type::DELETE:
        try {
            rope.rdelete(oper.beg, oper.end - oper.beg);
        } catch (const std::exception &ex) {
            std::cout << "rdelete exception: can't rdelete on indexes " << oper.beg << " and " << oper.end;
        }
        break;
    }
}

void History::undo(Rope &rope)
{
    if (_ops.empty() || _current_index <= -1)
        return;
    
    op oper = _ops.at(_current_index);
    --_current_index;

    oper.type = (oper.type == op_type::INSERT) ? (op_type::DELETE) : (op_type::INSERT);
    execute(oper, rope);
}

void History::redo(Rope &rope)
{
    if (_ops.empty() || _current_index == _ops.size() - 1)
        return;

    ++_current_index;
    op oper = _ops.at(_current_index);
    //++_current_index;

    execute(oper, rope);
}
