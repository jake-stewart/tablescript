#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

enum TokenType {
    TOKEN_TEXT = 0,
    TOKEN_COL_SEP = 1,
    TOKEN_ROW_SEP = 2,
    TOKEN_H_MERGE = 3,
    TOKEN_V_MERGE = 4,
    TOKEN_STYLE = 5,
    TOKEN_INLINE_CODE = 6,
    TOKEN_CODE_BLOCK = 7,
    TOKEN_H_BORDER = 8,
    TOKEN_THICK_H_BORDER = 9
};

#endif
