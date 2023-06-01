#ifndef TABLE_WIDTH_SIZER_HPP
#define TABLE_WIDTH_SIZER_HPP

#include "cell.hpp"
#include "table.hpp"
#include "table_word_wrapper.hpp"
#include <optional>
#include <vector>

struct TableWidthConfig
{
    std::optional<int> width;
    std::optional<int> maxWidth;
    std::optional<std::vector<int>> columnWidths;
    std::optional<std::vector<int>> maxColumnWidths;
};

class TableWidthSizer
{
    TableWidthConfig _config;
    Table &_table;
    TableWordWrapper _wrapper;
    std::vector<int> _maxContentWidths;
    int _remainingWidth;
    int _width;
    int _maxContentWidth;
    int _columnWidthPortion;

public:
    TableWidthSizer(Table &table) : _wrapper(table), _table(table) {}
    void setWidthConfig(TableWidthConfig config);
    void applySizing();

private:
    void _sizeToExactWidths();
    std::vector<int> _validatedMaxWidths();
    void _sizeToFlexibleWidths();
    std::vector<int> _getWidths();
    std::vector<int> _calcMaxContentColumnWidths();
    bool _collapseWidths();
    void _applyTableSizing();
    void _capWidthsToTarget();
    void _calcRemainingWidth();
    void _applyRatioRemainingToLargerColumns();
    void _scatterRemainingToLargerColumns();
    void _scatterRemainingWidth();
    void _calcWidthDistribution();
};

#endif
