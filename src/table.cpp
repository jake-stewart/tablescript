#include "../include/table.hpp"

TableRow &Table::operator[](int idx) {
    return rows[idx];
}
