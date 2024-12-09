#include "op.h"

bool op::operator==(const op& another) const
{
    return (this->type == another.type) &&
           (this->substr == another.substr) &&
           (this->beg == another.beg) &&
           (this->end == another.end);
}

std::string op::to_string()
{
    std::string repres;

    switch (type) {
    case op_type::INSERT:
        repres += "INSERT";
        break;
    case op_type::DELETE:
        repres += "DELETE";
        break;
    case op_type::UNKNOWN:
        repres += "UNKNOWN";
        break;
    }

    repres = repres + " " + substr + " " + std::to_string(beg) + " " + std::to_string(end);

    return repres;
}