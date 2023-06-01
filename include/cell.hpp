#ifndef CELL_HPP
#define CELL_HPP

#include "alignment.hpp"
#include "cell_content.hpp"
#include "cell_style.hpp"
#include "util.hpp"
#include "vec2.hpp"
#include <optional>
#include <string>
#include <tuple>
#include <vector>

struct Cell
{
    std::optional<Vec2<int>> merge;
    int rowspan = 1;
    int colspan = 1;
    CellStyle style;
    std::vector<CellContent> input;
    std::string output;
    std::vector<std::string> lines;
};

#endif
