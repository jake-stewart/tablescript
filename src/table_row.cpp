#include "../include/table_row.hpp"

Cell &TableRow::operator[](int idx) {
    return cells[idx];
}
