#ifndef TABLE_AUTO_FORMATTER_HPP
#define TABLE_AUTO_FORMATTER_HPP

#include "cell.hpp"
#include "table.hpp"
#include <regex>

const std::regex NUMBER_REGEX("\\s*\\$?[0-9]+(\\.[0-9]+)?\\s*");

class TableAutoFormatter
{
    Table &_table;

public:
    TableAutoFormatter(Table &table) : _table(table) {}
    void format();

private:
    void _formatRow(int row);
    void _formatCell(Cell &cell);
};

#endif
