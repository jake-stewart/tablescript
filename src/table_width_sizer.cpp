#include "../include/table_width_sizer.hpp"

void TableWidthSizer::setWidthConfig(TableWidthConfig config) {
    _config = config;
}

void TableWidthSizer::applySizing() {
    if (_config.columnWidths) {
        _sizeToExactWidths();
    }
    else {
        _sizeToFlexibleWidths();
    }
}

void TableWidthSizer::_sizeToExactWidths() {
    std::vector<int> stretchColumns;
    int end = std::min(_config.columnWidths->size(), _table[0].cells.size());
    for (int i = 0; i < end; i++) {
        int width = _config.columnWidths->at(i);
        if (width > 0) {
            _table.columnWidths.push_back(width);
        }
        else {
            _table.columnWidths.push_back(0);
            stretchColumns.push_back(i);
        }
    }
    for (int i = end; i < _table[0].cells.size(); i++) {
        _table.columnWidths.push_back(0);
        stretchColumns.push_back(i);
    }
    if (!stretchColumns.size()) {
        _wrapper.applyWordWrap();
        return;
    }
    int width = _config.width
        ? _config.width.value()
        : (_config.maxWidth ? _config.maxWidth.value() : 80);
    int specifiedWidth = sum(_config.columnWidths.value(), 0, end);
    int remainingWidth = width - specifiedWidth -
        _table[0].cells.size() * _table.padding * 2 -
        (_table[0].cells.size() - 1) - 2;
    int widthPerColumn = remainingWidth / stretchColumns.size();
    widthPerColumn = std::max(widthPerColumn, 1);
    for (int i : stretchColumns) {
        _table.columnWidths[i] += widthPerColumn;
    }
    remainingWidth -= stretchColumns.size() * widthPerColumn;
    while (remainingWidth > 0) {
        for (int i : stretchColumns) {
            _table.columnWidths[i]++;
            if (--remainingWidth <= 0) {
                break;
            }
        }
    }
    _wrapper.applyWordWrap();
}

std::vector<int> TableWidthSizer::_validatedMaxWidths() {
    std::vector<int> maxWidths(_table[0].cells.size(), 100000);
    if (_config.maxColumnWidths) {
        int end = std::min(
            _table[0].cells.size(), _config.maxColumnWidths->size()
        );
        for (int i = 0; i < end; i++) {
            int width = _config.maxColumnWidths.value()[i];
            maxWidths[i] = width > 0 ? width : 100000;
        }
    }
    return maxWidths;
}

void TableWidthSizer::_sizeToFlexibleWidths() {
    _table.columnWidths = _config.maxColumnWidths
        ? _validatedMaxWidths()
        : std::vector<int>(_table[0].cells.size(), 100000);
    _wrapper.applyWordWrap();
    std::vector<int> maxContentWidths = _calcMaxContentColumnWidths();
    _table.columnWidths = maxContentWidths;
    _applyTableSizing();
    if (_table.columnWidths != maxContentWidths) {
        _wrapper.applyWordWrap();
        if (!_config.width) {
            while (_collapseWidths()) {
                ;
            }
        }
    }
}

std::vector<int> TableWidthSizer::_getWidths() {
    std::vector<int> widths(_table[0].cells.size(), 0);
    for (int i = 0; i < _table.rows.size(); i++) {
        TableRow &row = _table[i];
        int col = 0;
        while (col < _table[0].cells.size()) {
            Cell &cell = row.cells[col];
            if (cell.colspan > 1) {
                col += cell.colspan;
                continue;
            }
            for (const std::string &line : cell.lines) {
                widths[col] = std::max(widths[col], (int)line.length());
            }
            col++;
        }
    }
    return widths;
}

std::vector<int> TableWidthSizer::_calcMaxContentColumnWidths() {
    std::vector<int> widths = _getWidths();
    for (TableRow &row : _table.rows) {
        int col = 0;
        while (col < row.cells.size()) {
            Cell &cell = row.cells[col];
            if (cell.colspan <= 1 || cell.merge) {
                col += 1;
                continue;
            }
            int longestLine = 0;
            for (std::string &line : cell.lines) {
                longestLine = std::max(longestLine, (int)line.length());
            }
            int available = sum(widths, col, cell.colspan);
            available += (cell.colspan - 1) * (_table.padding * 2 + 1);
            if (longestLine > available) {
                widths[col] = longestLine;
            }
            col += cell.colspan;
        }
    }
    return widths;
}

bool TableWidthSizer::_collapseWidths() {
    std::vector<int> excessWidths = _table.columnWidths;

    for (int i = 0; i < _table.rows.size(); i++) {
        TableRow &row = _table[i];
        int col = 0;
        while (col < row.cells.size()) {
            Cell &cell = row.cells[col];
            if (cell.merge) {
                col++;
                continue;
            }
            int longestLine = 0;
            for (const std::string &line : cell.lines) {
                longestLine = std::max((int)line.length(), longestLine);
            }
            int available = sum(_table.columnWidths, col, cell.colspan);
            available += (cell.colspan - 1) * (_table.padding * 2 + 1);
            int excess = available - longestLine;
            for (int i = 0; i < cell.colspan; i++) {
                excessWidths[col + i] = std::min(excess, excessWidths[col + i]);
            }
            col += cell.colspan;
        }
    }

    bool hasExcess = false;

    while (true) {
        int largest = 0;
        int largestIdx;
        for (int i = 0; i < excessWidths.size(); i++) {
            if (excessWidths[i] > largest) {
                largestIdx = i;
                largest = excessWidths[i];
            }
            excessWidths[i]--;
        }
        if (largest <= 0) {
            break;
        }
        hasExcess = true;
        _table.columnWidths[largestIdx]--;
    }

    return hasExcess;
}

void TableWidthSizer::_applyTableSizing() {
    if (!_config.maxWidth && !_config.width) {
        return;
    }

    int numColumns = _table.columnWidths.size();
    _maxContentWidths = _table.columnWidths;
    _maxContentWidth = sum(_table.columnWidths);

    if (_config.width) {
        _width = _config.width.value() - numColumns - 1 -
            (numColumns * _table.padding * 2);
        _columnWidthPortion = std::max(_width / numColumns, 1);
    }
    else if (_config.maxWidth) {
        _width = _config.maxWidth.value() - numColumns - 1 -
            (numColumns * _table.padding * 2);
        if (_maxContentWidth <= _width) {
            return;
        }
        _columnWidthPortion = std::max(
            1, std::min(_width, _maxContentWidth) / numColumns
        );
    }

    _calcWidthDistribution();
}

void TableWidthSizer::_capWidthsToTarget() {
    for (int i = 0; i < _maxContentWidths.size(); i++) {
        _table.columnWidths[i] = std::min(
            _columnWidthPortion, _maxContentWidths[i]
        );
    }
}

void TableWidthSizer::_calcRemainingWidth() {
    _remainingWidth = _width - sum(_table.columnWidths);
}

void TableWidthSizer::_applyRatioRemainingToLargerColumns() {
    for (int i = 0; i < _maxContentWidths.size(); i++) {
        if (_maxContentWidths[i] <= _columnWidthPortion) {
            continue;
        }
        float ratio = (float)_maxContentWidths[i] / _maxContentWidth;
        int extraWidth = ratio * _remainingWidth;
        _table.columnWidths[i] += extraWidth;
        if (_table.columnWidths[i] < 1) {
            _table.columnWidths[i] = 1;
        }
        _remainingWidth -= extraWidth;
    }
}

void TableWidthSizer::_scatterRemainingToLargerColumns() {
    while (_remainingWidth > 0) {
        bool largeColumnFound = false;
        for (int i = 0; i < _maxContentWidths.size(); i++) {
            if (_table.columnWidths[i] >= _maxContentWidths[i]) {
                continue;
            }
            largeColumnFound = true;
            _table.columnWidths[i]++;
            if (--_remainingWidth == 0) {
                break;
            }
        }
        if (!largeColumnFound) {
            break;
        }
    }
}

void TableWidthSizer::_scatterRemainingWidth() {
    while (_remainingWidth > 0) {
        for (int i = 0; i < _maxContentWidths.size(); i++) {
            _table.columnWidths[i]++;
            if (--_remainingWidth == 0) {
                break;
            }
        }
    }
}

void TableWidthSizer::_calcWidthDistribution() {
    _capWidthsToTarget();
    _calcRemainingWidth();
    _applyRatioRemainingToLargerColumns();
    _scatterRemainingToLargerColumns();
    _scatterRemainingWidth();
}
