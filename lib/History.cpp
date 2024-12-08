#include "History.h"

#include <iostream>

const std::size_t History::hist_size = 25;

Stack History::get_undo() { return _undo; }
Stack History::get_redo() { return _redo;}

void History::add(op oper, Rope &rope)
{
    _undo.push(oper);
    _redo.clear();

    execute(oper, rope);
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
    if (_undo.empty())
        return;
    
    op oper = _undo.top();
    _undo.pop();

    _redo.push(oper);
    oper.type = (oper.type == op_type::INSERT) ? (op_type::DELETE) : (op_type::INSERT);
    execute(oper, rope);
}

void History::redo(Rope &rope)
{
    if (_redo.empty())
        return;

    op oper = _redo.top();
    _redo.pop();

    _undo.push(oper);;
    execute(oper, rope);
}
