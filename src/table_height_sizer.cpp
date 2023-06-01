#include "../include/table_height_sizer.hpp"

void TableHeightSizer::applySizing() {
    std::vector<int> heights = _getHeights();
    std::vector<ExcessRange> ranges = _getExcessHeightRanges(heights);
    if (ranges.size()) {
        std::vector<int> increases = excessOverRanges(
            ranges, _table.rows.size()
        );
        for (int i = 0; i < increases.size(); i++) {
            heights[i] += increases[i];
        }
    }
    _table.rowHeights = heights;
}

std::vector<int> TableHeightSizer::_getHeights() {
    std::vector<int> heights(_table.rows.size());
    for (int row = 0; row < _table.rows.size(); row++) {
        int col = 0;
        while (col < _table.rows[row].cells.size()) {
            Cell &cell = _table[row][col];
            col += cell.colspan;
            if (cell.merge || cell.rowspan > 1) {
                continue;
            }
            heights[row] = std::max(heights[row], (int)cell.lines.size());
        }
    }
    return heights;
}

std::vector<ExcessRange> TableHeightSizer::_getExcessHeightRanges(
    std::vector<int> &heights
) {
    std::vector<ExcessRange> ranges;
    for (int row = 0; row < _table.rows.size(); row++) {
        int col = 0;
        while (col < _table[row].cells.size()) {
            Cell &cell = _table[row][col];
            col += cell.colspan;
            if (cell.merge || cell.rowspan <= 1) {
                continue;
            }
            int totalHeight = sum(heights, row, cell.rowspan);
            totalHeight += cell.rowspan - 1;
            int excessHeight = cell.lines.size() - totalHeight;
            if (excessHeight > 0) {
                ranges.push_back({row, row + cell.rowspan, excessHeight});
            }
        }
    }
    return ranges;
}
