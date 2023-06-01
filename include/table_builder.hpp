#ifndef TABLE_BUILDER_HPP
#define TABLE_BUILDER_HPP

#include "table.hpp"
#include "vec2.hpp"
#include <set>

class TableBuilder
{
    Table &_table;

public:
    TableBuilder(Table &table) : _table(table) {}
    void setScript(std::string script);
    void addCellContent(int row, int col, CellContent content);
    void addCellStyle(int row, int col, CellStyle style);
    void addHorizontalBorder(int row);
    void addThickHorizontalBorder(int row);
    void mergeCellWithAbove(int row, int col);
    void mergeCellWithLeft(int row, int col);
    void fixFullMerges();
    void deleteEmptyRows();
    void deleteEmptyColumns();

private:
    void _merge(Vec2<int> a, Vec2<int> b);
    void _expandToFit(int row, int col);
    void _addRow();
    void _addCol();
    void _fixFullRowMerges();
    void _deleteRow(int row);
    void _deleteCell(int row, int col);
    void _fixFullColumnMerges();
};

#endif
