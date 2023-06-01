#ifndef TABLE_WORD_WRAPPER_HPP
#define TABLE_WORD_WRAPPER_HPP

#include "table.hpp"
#include "util.hpp"
#include <string>
#include <vector>

class TableWordWrapper
{
    Table &_table;

public:
    TableWordWrapper(Table &table) : _table(table) {}
    void applyWordWrap();

private:
    void _wrapCell(int row, int col);
};

#endif
