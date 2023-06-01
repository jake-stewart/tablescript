#include "../include/table_printer.hpp"

void TablePrinter::print() {
    _border.setStyle(_table.border);
    _border.setSeparatorStyle(_table.separator);
    _pad = repeat(" ", _table.padding);

    _populateColumnSweeper();
    _printTopBorder();
    for (int row = 0; row < _table.rowHeights.size(); row++) {
        _printRow(row);
        if (row < _table.rows.size() - 1) {
            _printRowBorder(row);
            _prepareRow(row + 1);
        }
    }
    _printBottomBorder();
}

void TablePrinter::_resetColumnSweeper(ColumnSweeper &sweeper, int row) {
    sweeper.rowSpanProgress = 0;
    sweeper.cell = &_table[row][sweeper.col];
    // sweeper.width = sum(_table.columnWidths[sweeper.col:sweeper.col +
    // sweeper.cell->colspan]);
    sweeper.width = sum(
        _table.columnWidths, sweeper.col, sweeper.cell->colspan
    );
    sweeper.width += sweeper.cell->colspan - 1;
    sweeper.width += sweeper.cell->colspan * _table.padding * 2;
    // sweeper.height = sum(_table.rowHeights[row:row +
    // sweeper.cell->rowspan]);
    sweeper.height = sum(_table.rowHeights, row, sweeper.cell->rowspan);
    sweeper.height += sweeper.cell->rowspan - 1;

    switch (sweeper.cell->style.valign) {
        case V_ALIGN_TOP:
            sweeper.line = 0;
            break;
        case V_ALIGN_MIDDLE:
            sweeper.line = -(sweeper.height - sweeper.cell->lines.size()) / 2;
            break;
        case V_ALIGN_BOTTOM:
            sweeper.line = -(sweeper.height - sweeper.cell->lines.size());
            break;
    }
}

void TablePrinter::_populateColumnSweeper() {
    _columnSweepers.clear();
    for (int i = 0; i < _table.rows[0].cells.size(); i++) {
        _columnSweepers.push_back(ColumnSweeper());
        _columnSweepers[i].cell = &_table[0][i];
        _columnSweepers[i].col = i;
        _resetColumnSweeper(_columnSweepers[i], 0);
    }
}

void TablePrinter::_printTopBorder() {
    bool thick = _table.topBorder == 2;
    printf("%s", _border.topLeft(thick));
    int col = 0;
    for (ColumnSweeper &sweeper : _columnSweepers) {
        if (sweeper.cell->merge) {
            continue;
        }
        const char *c = _border.horizontal(thick);
        printf("%s", repeat(c, sweeper.width).c_str());
        col += sweeper.cell->colspan;
        if (col < _columnSweepers.size()) {
            printf("%s", _border.top(thick));
        }
    }
    printf("%s\n", _border.topRight(thick));
}

void TablePrinter::_printBottomBorder() {
    bool thick = _table.rows[_table.rows.size() - 1].borderBottom == 2;
    int col = 0;
    printf("%s", _border.bottomLeft(thick));
    for (ColumnSweeper &sweeper : _columnSweepers) {
        if (sweeper.cell->merge) {
            continue;
        }
        const char *c = _border.horizontal(thick);
        printf("%s", repeat(c, sweeper.width).c_str());
        col += sweeper.cell->colspan;
        if (col < _columnSweepers.size()) {
            printf("%s", _border.bottom(thick));
        }
    }
    printf("%s\n", _border.bottomRight(thick));
}

void TablePrinter::_printRow(int row) {
    for (int i = 0; i < _table.rowHeights[row]; i++) {
        for (ColumnSweeper &sweeper : _columnSweepers) {
            if (sweeper.cell->merge) {
                continue;
            }
            printf("%s", _border.vertical(false));
            _printCellContent(sweeper);
        }
        printf("%s\n", _border.vertical(false));
    }
}

void TablePrinter::_printCellContent(ColumnSweeper &sweeper) {
    std::string line;
    if (0 <= sweeper.line && sweeper.line < sweeper.cell->lines.size()) {
        line = sweeper.cell->lines[sweeper.line];
    }
    else {
        line = "";
    }
    int width = sweeper.width - _table.padding * 2;
    int padLeft;
    int padRight;
    switch (sweeper.cell->style.align) {
        case H_ALIGN_LEFT:
            printf("%s%-*s%s", _pad.c_str(), width, line.c_str(), _pad.c_str());
            padLeft = width - line.length();
            padRight = 0;
            break;
        case H_ALIGN_RIGHT:
            printf("%s%*s%s", _pad.c_str(), width, line.c_str(), _pad.c_str());
            padLeft = 0;
            padRight = width - line.length();
            break;
        case H_ALIGN_CENTER:
            padLeft = (width - line.length()) / 2;
            padRight = width - line.length() - padLeft;
            printf(
                "%-*s%s%*s", padLeft + _table.padding, "", line.c_str(),
                padRight + _table.padding, ""
            );
            break;
    }
    sweeper.line += 1;
}

void TablePrinter::_printRowBorderSegment(int row, int col, bool thick) {
    ColumnSweeper &sweeper = _columnSweepers[col];
    const char *glyph = _border.horizontal(thick);
    printf(
        "%s",
        repeat(glyph, _table.columnWidths[sweeper.col] + _table.padding * 2)
            .c_str()
    );
    if (col >= _columnSweepers.size() - 1) {
        printf("%s", _border.right(thick));
        return;
    }
    ColumnSweeper *nextSweeper = &_columnSweepers[col + 1];
    if (nextSweeper->rowSpanProgress != nextSweeper->cell->rowspan - 1) {
        printf("%s", _border.right(thick));
        return;
    }
    Cell *nextCell = &_table[row + 1][sweeper.col + 1];
    if (nextSweeper->cell->merge) {
        if (nextCell->merge) {
            printf("%s", _border.horizontal(thick));
        }
        else {
            printf("%s", _border.top(thick));
        }
    }
    else {
        if (nextCell->merge) {
            printf("%s", _border.bottom(thick));
        }
        else {
            printf("%s", _border.middle(thick));
        }
    }
}

void TablePrinter::_printRowBorderContent(ColumnSweeper &sweeper, bool thick) {
    _printCellContent(sweeper);
    if (sweeper.col < _columnSweepers.size() - 1) {
        ColumnSweeper *nextSweeper = &_columnSweepers[sweeper.col + 1];
        while (nextSweeper->cell->merge) {
            nextSweeper = &_columnSweepers[nextSweeper->col + 1];
        }
        if (nextSweeper->rowSpanProgress + 1 == nextSweeper->cell->rowspan) {
            printf("%s", _border.left(thick));
        }
        else {
            printf("%s", _border.vertical(thick));
        }
    }
    else {
        printf("%s", _border.vertical(thick));
    }
}

void TablePrinter::_printRowBorder(int row) {
    if (!_table.rows[row].borderBottom) {
        return;
    }
    bool thick = _table.rows[row].borderBottom == 2;
    ColumnSweeper &sweeper = _columnSweepers[0];
    if (sweeper.rowSpanProgress == sweeper.cell->rowspan - 1) {
        printf("%s", _border.left(thick));
    }
    else {
        printf("%s", _border.vertical(thick));
    }

    int col = 0;
    while (col < _columnSweepers.size()) {
        ColumnSweeper &sweeper = _columnSweepers[col];
        if (sweeper.cell->rowspan == sweeper.rowSpanProgress + 1) {
            _printRowBorderSegment(row, col, thick);
            col += 1;
        }
        else if (sweeper.cell->merge) {
            col += 1;
        }
        else {
            _printRowBorderContent(sweeper, thick);
            col += sweeper.cell->colspan;
        }
    }
    printf("\n");
}

void TablePrinter::_prepareRow(int row) {
    if (row >= _table.rows.size()) {
        return;
    }
    for (ColumnSweeper &sweeper : _columnSweepers) {
        sweeper.rowSpanProgress += 1;
        if (sweeper.cell->rowspan == sweeper.rowSpanProgress) {
            _resetColumnSweeper(sweeper, row);
        }
    }
}
