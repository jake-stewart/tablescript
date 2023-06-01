#include "../include/table_builder.hpp"

void TableBuilder::setScript(std::string script) {
    _table.script = script;
}

void TableBuilder::addCellContent(int row, int col, CellContent content) {
    _expandToFit(row, col);
    _table[row][col].input.push_back(content);
}

void TableBuilder::addCellStyle(int row, int col, CellStyle style) {
    _expandToFit(row, col);
    _table[row][col].style = style;
}

void TableBuilder::addHorizontalBorder(int row) {
    if (row == -1) {
        _table.topBorder = 1;
    }
    else {
        _table.rows[row].borderBottom = 1;
    }
}

void TableBuilder::addThickHorizontalBorder(int row) {
    if (row == -1) {
        _table.topBorder = 2;
    }
    else {
        _table[row].borderBottom = 2;
    }
}

void TableBuilder::mergeCellWithAbove(int row, int col) {
    if (row == 0) {
        return;
    }
    _merge({row, col}, {row - 1, col});
}

void TableBuilder::mergeCellWithLeft(int row, int col) {
    if (col == 0) {
        return;
    }
    _merge({row, col}, {row, col - 1});
}

void TableBuilder::fixFullMerges() {
    if (_table.rows.size() == 0) {
        return;
    }
    _fixFullRowMerges();
    _fixFullColumnMerges();
}

void TableBuilder::deleteEmptyRows() {
    for (int i = _table.rows.size() - 1; i >= 0; i--) {
        bool canDelete = true;
        for (Cell &cell : _table[i].cells) {
            bool hasContent = cell.output.size() ||
                (cell.merge && cell.merge->x != i || cell.rowspan > 1);
            if (hasContent) {
                canDelete = false;
                break;
            }
        }
        if (canDelete) {
            if (i == 0) {
                if (_table.rows[i].borderBottom) {
                    _table.topBorder = _table.rows[i].borderBottom;
                }
            }
            else {
                if (_table.rows[i].borderBottom) {
                    _table.rows[i - 1].borderBottom = _table.rows[i]
                                                          .borderBottom;
                }
            }
            _table.rows.erase(_table.rows.begin() + i);
        }
    }
}

void TableBuilder::deleteEmptyColumns() {
    for (int i = _table[0].cells.size() - 1; i >= 0; i--) {
        bool canDelete = true;
        for (TableRow &row : _table.rows) {
            Cell &cell = row[i];
            bool hasContent = cell.output.size() ||
                (cell.merge && cell.merge->y != i || cell.colspan > 1);
            if (hasContent) {
                canDelete = false;
                break;
            }
        }
        if (canDelete) {
            for (TableRow &row : _table.rows) {
                row.cells.erase(row.cells.begin() + i);
            }
        }
    }
}

void TableBuilder::_merge(Vec2<int> a, Vec2<int> b) {
    Vec2<int> topLeft = std::min(a, b);
    Vec2<int> bottomRight = std::max(a, b);
    _expandToFit(bottomRight.x, bottomRight.y);
    Cell *topLeftCell = &_table[topLeft.x][topLeft.y];
    if (topLeftCell->merge) {
        topLeft = topLeftCell->merge.value();
        topLeftCell = &_table[topLeft.x][topLeft.y];
    }
    topLeftCell->rowspan = std::max(
        topLeftCell->rowspan, bottomRight.x - topLeft.x + 1
    );
    topLeftCell->colspan = std::max(
        topLeftCell->colspan, bottomRight.y - topLeft.y + 1
    );
    for (int i = 0; i < topLeftCell->rowspan; i++) {
        for (int j = 0; j < topLeftCell->colspan; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            _table[topLeft.x + i][topLeft.y + j].merge = topLeft;
        }
    }

    for (int i = topLeft.x; i < topLeft.x + topLeftCell->rowspan; i++) {
        for (int j = topLeft.y; j < topLeft.y + topLeftCell->colspan; j++) {
            if (Vec2(i, j) == topLeft) {
                continue;
            }
            _table[i][j].merge = topLeft;
        }
    }
}

void TableBuilder::_expandToFit(int row, int col) {
    while (row >= _table.rows.size()) {
        _addRow();
    }
    while (col >= _table.rows[0].cells.size()) {
        _addCol();
    }
}

void TableBuilder::_addRow() {
    int row = _table.rows.size();
    _table.rows.push_back(TableRow());
    for (int i = 0; i < _table.rows[0].cells.size(); i++) {
        _table.rows[row].cells.push_back(Cell());
    }
}

void TableBuilder::_addCol() {
    for (TableRow &row : _table.rows) {
        row.cells.push_back(Cell());
    }
}

void TableBuilder::_fixFullRowMerges() {
    for (int i = _table.rows.size() - 1; i >= 0; i--) {
        bool fullMerge = true;
        for (int j = 0; j < _table.rows[0].cells.size(); j++) {
            if (!_table[i][j].merge) {
                fullMerge = false;
                break;
            }
        }
        if (fullMerge) {
            std::set<Cell *> affectedCells;
            for (int j = 0; j < _table[0].cells.size(); j++) {
                Vec2<int> mergedInto = _table[i][j].merge.value();
                affectedCells.insert(&_table[mergedInto.x][mergedInto.y]);
            }
            _deleteRow(i);
            for (Cell *affectedCell : affectedCells) {
                affectedCell->rowspan -= 1;
            }
        }
    }
}

void TableBuilder::_deleteRow(int row) {
    _table.rows.erase(_table.rows.begin() + row);
}

void TableBuilder::_deleteCell(int row, int col) {
    _table[row].cells.erase(_table[row].cells.begin() + col);
}

void TableBuilder::_fixFullColumnMerges() {
    for (int i = _table[0].cells.size() - 1; i >= 0; i--) {
        bool fullMerge = true;
        for (int j = 0; j < _table.rows.size(); j++) {
            if (!_table[j][i].merge) {
                fullMerge = false;
                break;
            }
        }
        if (fullMerge) {
            std::set<Cell *> affectedCells;
            for (int j = 0; j < _table.rows.size(); j++) {
                Vec2<int> mergedInto = _table[j][i].merge.value();
                affectedCells.insert(&_table[mergedInto.x][mergedInto.y]);
                _deleteCell(j, i);
            }
            for (Cell *affectedCell : affectedCells) {
                affectedCell->colspan -= 1;
            }
        }
    }
}
