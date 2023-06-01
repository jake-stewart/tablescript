#ifndef TABLE_HEIGHT_SIZER_HPP
#define TABLE_HEIGHT_SIZER_HPP

#include "excess_over_range.hpp"
#include "table.hpp"
#include "util.hpp"
#include <vector>

class TableHeightSizer
{
    Table &_table;

public:
    TableHeightSizer(Table &table) : _table(table) {}
    void applySizing();

private:
    std::vector<int> _getHeights();
    std::vector<ExcessRange> _getExcessHeightRanges(std::vector<int> &heights);
};

#endif
