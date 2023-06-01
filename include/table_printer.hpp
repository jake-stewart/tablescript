#ifndef TABLE_PRINTER_HPP
#define TABLE_PRINTER_HPP

#include "border.hpp"
#include "cell.hpp"
#include "cell_style.hpp"
#include "table.hpp"
#include "util.hpp"

struct ColumnSweeper
{
    int rowSpanProgress = 0;
    int effectiveRow = 0;
    int line = 0;
    Cell *cell;
    int col = 0;
    int width = 0;
    int height = 0;
};

class TablePrinter
{
    Table &_table;
    std::vector<ColumnSweeper> _columnSweepers;
    Border _border;
    std::string _pad;

public:
    TablePrinter(Table &table) : _table(table) {}
    void print();

private:
    void _resetColumnSweeper(ColumnSweeper &sweeper, int row);
    void _populateColumnSweeper();
    void _printTopBorder();
    void _printBottomBorder();
    void _printRow(int row);
    void _printCellContent(ColumnSweeper &sweeper);
    void _printRowBorderSegment(int row, int col, bool thick);
    void _printRowBorderContent(ColumnSweeper &sweeper, bool thick);
    void _printRowBorder(int row);
    void _prepareRow(int row);
};

#endif
