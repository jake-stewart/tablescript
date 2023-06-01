#ifndef CELL_STYLE_HPP
#define CELL_STYLE_HPP

#include "alignment.hpp"
#include <string>

struct CellStyle
{
    HorizontalAlignment align = H_ALIGN_LEFT;
    VerticalAlignment valign = V_ALIGN_TOP;
    int padding = 1;
    bool isDefault = true;

    static CellStyle parse(std::string schema);
};

#endif
