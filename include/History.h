#ifndef HISTORY_H
#define HISTORY_H

#include <vector>

#include "op.h"
#include "Rope/Rope.h"
#include "Stack/Stack.h"

class History {
public:
    static const std::size_t hist_size;
    
    History() = default;

    void add(op oper, Rope &rope);
    void execute(op oper, Rope &rope);

    void undo(Rope &rope);
    void redo(Rope &rope);

    Stack get_undo();
    Stack get_redo();

private:
    Stack _undo;
    Stack _redo;
};

#endif
