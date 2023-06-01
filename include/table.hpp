#ifndef TABLE_HPP
#define TABLE_HPP

#include <string>
#include <vector>

#include "border.hpp"
#include "cell.hpp"
#include "table_row.hpp"

struct Table
{
    std::string script;
    std::vector<int> columnWidths;
    std::vector<int> rowHeights;
    std::vector<TableRow> rows;
    int padding = 1;
    int topBorder = 0;
    BorderStyle border;
    BorderStyle separator;

    TableRow &operator[](int idx);
};

#endif
