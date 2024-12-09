#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <deque>

#include "op.h"
#include "Rope/Rope.h"
#include "List/List.h"

class History {
public:
    static const std::size_t hist_size;
    
    History() : _current_index{-1} {}

    List get_list();
    int  get_current_index();

    void add(op oper, Rope &rope);
    void execute(op oper, Rope &rope);

    void undo(Rope &rope);
    void redo(Rope &rope);

private:
    List _ops;
    int  _current_index;
};

#endif
