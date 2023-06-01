#ifndef TABLE_ROW_HPP
#define TABLE_ROW_HPP

#include "cell.hpp"

struct TableRow
{
    std::vector<Cell> cells;
    int borderBottom;

    Cell &operator[](int idx);
};

#endif
