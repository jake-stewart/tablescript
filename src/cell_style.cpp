#include "../include/cell_style.hpp"

CellStyle CellStyle::parse(std::string schema) {
    CellStyle style;
    style.isDefault = false;
    for (char c : schema) {
        switch (c) {
            case 'l':
                style.align = H_ALIGN_LEFT;
                break;
            case 'c':
                style.align = H_ALIGN_CENTER;
                break;
            case 'r':
                style.align = H_ALIGN_RIGHT;
                break;
            case 't':
                style.valign = V_ALIGN_TOP;
                break;
            case 'm':
                style.valign = V_ALIGN_MIDDLE;
                break;
            case 'b':
                style.valign = V_ALIGN_BOTTOM;
                break;
        }
    }
    return style;
}
