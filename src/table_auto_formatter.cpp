#include "../include/table_auto_formatter.hpp"

void TableAutoFormatter::format() {
    for (int i = 0; i < _table.rows.size(); i++) {
        _formatRow(i);
    }
}

void TableAutoFormatter::_formatRow(int row) {
    for (int j = 0; j < _table[row].cells.size(); j++) {
        Cell &cell = _table[row][j];
        if (!cell.merge && !_table[row].borderBottom) {
            if (cell.colspan > 1 || cell.rowspan > 1) {
                _table[row].borderBottom = 1;
                if (row > 0 && !_table[row - 1].borderBottom) {
                    _table[row - 1].borderBottom = 1;
                }
            }
        }

        _formatCell(cell);
    }
}

void TableAutoFormatter::_formatCell(Cell &cell) {
    if (!cell.style.isDefault) {
        return;
    }
    if (cell.lines.size() != 1) {
        return;
    }
    if (std::regex_match(cell.lines[0], NUMBER_REGEX)) {
        cell.style.align = H_ALIGN_RIGHT;
    }
}
