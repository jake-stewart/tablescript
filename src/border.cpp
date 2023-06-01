#include "../include/border.hpp"

void Border::setStyle(BorderStyle style) {
    _style = style;
    StyleCharLookup &lookup = STYLES.at(_style);
    _chars[0] = lookup.at(_style);
}

void Border::setSeparatorStyle(BorderStyle separatorStyle) {
    if (STYLES.at(_style).contains(separatorStyle)) {
        _chars[1] = STYLES.at(_style).at(separatorStyle);
    }
    else {
        _chars[1] = STYLES.at(separatorStyle).at(separatorStyle);
        _chars[1][VERTICAL] = _chars[0][VERTICAL];
        _chars[1][LEFT] = _chars[0][LEFT];
        _chars[1][RIGHT] = _chars[0][RIGHT];
    }
}

const char *Border::topLeft(bool thick) {
    return _chars[thick][TOP_LEFT];
}

const char *Border::topRight(bool thick) {
    return _chars[thick][TOP_RIGHT];
}

const char *Border::bottomLeft(bool thick) {
    return _chars[thick][BOTTOM_LEFT];
}

const char *Border::bottomRight(bool thick) {
    return _chars[thick][BOTTOM_RIGHT];
}

const char *Border::left(bool thick) {
    return _chars[thick][LEFT];
}

const char *Border::right(bool thick) {
    return _chars[thick][RIGHT];
}

const char *Border::top(bool thick) {
    return _chars[thick][TOP];
}

const char *Border::bottom(bool thick) {
    return _chars[thick][BOTTOM];
}

const char *Border::middle(bool thick) {
    return _chars[thick][MIDDLE];
}

const char *Border::horizontal(bool thick) {
    return _chars[thick][HORIZONTAL];
}

const char *Border::vertical(bool thick) {
    return _chars[thick][VERTICAL];
}
