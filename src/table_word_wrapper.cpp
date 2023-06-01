#include "../include/table_word_wrapper.hpp"

void TableWordWrapper::applyWordWrap() {
    for (int i = 0; i < _table.rows.size(); i++) {
        for (int j = 0; j < _table.rows[i].cells.size(); j++) {
            _wrapCell(i, j);
        }
    }
}

void TableWordWrapper::_wrapCell(int row, int col) {
    Cell &cell = _table[row][col];
    int width = sum(_table.columnWidths, col, cell.colspan) + cell.colspan - 1 +
        (cell.colspan - 1) * _table.padding * 2;

    std::string joined = deleteLeadingLinesOrWhitespace(cell.output);
    std::vector<std::string> paragraphs = split(rstrip(joined), '\n');
    cell.lines.clear();
    for (const std::string &paragraph : paragraphs) {
        if (paragraph.length()) {
            std::vector<std::string> lines = word_wrap(paragraph, width);
            cell.lines.insert(cell.lines.end(), lines.begin(), lines.end());
        }
    }
}
