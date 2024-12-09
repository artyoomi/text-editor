#ifndef OP_H
#define OP_H

#include <string>

enum class op_type {INSERT, DELETE, UNKNOWN};

// all information must be filled, to provide undo / redo functionality
struct op {
    op_type type;
    std::string substr;
    std::size_t beg;
    std::size_t end;

    op(op_type type_ = op_type::UNKNOWN, std::string substr_ = "", std::size_t beg_ = 0, std::size_t end_ = 0)
        : type{type_},
          substr{substr_},
          beg{beg_},
          end{end_}
    {}

    bool operator==(const op& another) const;

    std::string to_string();
};

#endif