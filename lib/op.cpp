#include "op.h"

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
    }

    repres = repres + " " + substr + " " + std::to_string(beg) + " " + std::to_string(end);

    return repres;
}