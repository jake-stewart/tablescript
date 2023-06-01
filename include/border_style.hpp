#ifndef BORDER_STYLE_HPP
#define BORDER_STYLE_HPP

#include <map>
#include <vector>

enum BorderStyle {
    SINGLE,
    DOUBLE,
    THICK,
    ASCII,
    THICK_ASCII,
    DOTTED,
    ROUND,
    BLOCK,
    DASHED,
    THICK_DASHED,
    NUM_BORDER_STYLES
};

enum BorderType {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    MIDDLE,
    HORIZONTAL,
    VERTICAL
};

typedef const std::map<BorderStyle, const std::vector<const char *>>
    StyleCharLookup;

const std::map<BorderStyle, StyleCharLookup> STYLES = {
    {SINGLE,
     {
         {SINGLE, {"┌", "┐", "└", "┘", "├", "┤", "┬", "┴", "┼", "─", "│"}},
         {DOUBLE, {"╒", "╕", "╘", "╛", "╞", "╡", "╤", "╧", "╪", "═", "│"}},
         {THICK, {"┍", "┑", "┕", "┙", "┝", "┥", "┯", "┷", "┿", "━", "│"}},
     }},
    {THICK,
     {
         {THICK, {"┏", "┓", "┗", "┛", "┣", "┫", "┳", "┻", "╋", "━", "┃"}},
         {SINGLE, {"┎", "┒", "┖", "┚", "┠", "┨", "┰", "┸", "╂", "─", "┃"}},
     }},
    {DOUBLE,
     {
         {DOUBLE, {"╔", "╗", "╚", "╝", "╠", "╣", "╦", "╩", "╬", "═", "║"}},
         {SINGLE, {"╓", "╖", "╙", "╜", "╟", "╢", "╥", "╨", "╫", "─", "║"}},
     }},
    {ROUND,
     {
         {ROUND, {"╭", "╮", "╰", "╯", "├", "┤", "┬", "┴", "┼", "─", "│"}},
         {SINGLE, {"╭", "╮", "╰", "╯", "├", "┤", "┬", "┴", "┼", "─", "│"}},
         {DOUBLE, {"╒", "╕", "╘", "╛", "╞", "╡", "╤", "╧", "╪", "═", "│"}},
         {THICK, {"┍", "┑", "┕", "┙", "┝", "┥", "┯", "┷", "┿", "━", "│"}},
     }},
    {ASCII,
     {
         {ASCII, {"+", "+", "+", "+", "+", "+", "+", "+", "+", "-", "|"}},
     }},
    {THICK_ASCII,
     {
         {THICK_ASCII, {"+", "+", "+", "+", "+", "+", "+", "+", "+", "=", "|"}},
     }},
    {DOTTED,
     {
         {DOTTED, {".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ":"}},
         {ASCII, {"+", "+", "+", "+", "+", "+", "-", "-", "+", "-", ":"}},
         {THICK_ASCII, {"+", "+", "+", "+", "+", "+", "=", "=", "+", "=", ":"}},
     }},
    {DASHED,
     {
         {DASHED, {"┌", "┐", "└", "┘", "├", "┤", "┬", "┴", "┼", "╌", "┆"}},
     }},
    {THICK_DASHED,
     {
         {THICK_DASHED,
          {"┏", "┓", "┗", "┛", "┣", "┫", "┳", "┻", "╋", "╍", "┇"}},
     }},
    {BLOCK,
     {
         {BLOCK, {"█", "█", "▀", "▀", "█", "█", "█", "▀", "█", "▀", "█"}},
     }},
};

#endif
