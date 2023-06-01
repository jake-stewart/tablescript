#ifndef BORDER_HPP
#define BORDER_HPP

#include "border_style.hpp"
#include <map>
#include <string>
#include <vector>

class Border
{
    std::vector<const char *> _chars[2];
    BorderStyle _style;

public:
    void setStyle(BorderStyle style);
    void setSeparatorStyle(BorderStyle separatorStyle);
    const char *topLeft(bool thick);
    const char *topRight(bool thick);
    const char *bottomLeft(bool thick);
    const char *bottomRight(bool thick);
    const char *left(bool thick);
    const char *right(bool thick);
    const char *top(bool thick);
    const char *bottom(bool thick);
    const char *middle(bool thick);
    const char *horizontal(bool thick);
    const char *vertical(bool thick);
};

#endif
