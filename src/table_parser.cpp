#include "../include/table_parser.hpp"

void TableParser::parse(std::vector<Token> *tokens) {
    _tokens = tokens;
    _row = 0;
    _col = 0;
    _idx = 0;
    _parse();
}

const Token *TableParser::_peek() {
    if (_idx >= _tokens->size()) {
        return nullptr;
    }
    return &_tokens->at(_idx);
}

const Token *TableParser::_next() {
    if (++_idx > _tokens->size()) {
        return nullptr;
    }
    return &_tokens->at(_idx - 1);
}

void TableParser::_parse() {
    const Token *token;
    while ((token = _next())) {
        _parseToken(token);
    }
}

void TableParser::_parseToken(const Token *token) {
    switch (token->type) {
        case TOKEN_TEXT:
            if (token->value.length()) {
                CellContent content({token->value, false});
                _builder.addCellContent(_row, _col, content);
            }
            break;
        case TOKEN_INLINE_CODE:
            if (token->value.length()) {
                CellContent content({token->value, true});
                _builder.addCellContent(_row, _col, content);
            }
            break;
        case TOKEN_CODE_BLOCK:
            if (token->value.length()) {
                _builder.setScript(token->value);
            }
            break;
        case TOKEN_STYLE:
            if (token->value.length()) {
                _parseStyleSchema(token->value);
            }
            break;
        case TOKEN_COL_SEP:
            _col++;
            break;
        case TOKEN_ROW_SEP:
            _row++;
            _col = 0;
            while ((token = _peek()) && token->type == TOKEN_ROW_SEP) {
                _next();
            }
            break;
        case TOKEN_H_MERGE:
            _builder.mergeCellWithLeft(_row, _col);
            break;
        case TOKEN_V_MERGE:
            _builder.mergeCellWithAbove(_row, _col);
            break;
        case TOKEN_H_BORDER:
            _builder.addHorizontalBorder(_row - 1);
            break;
        case TOKEN_THICK_H_BORDER:
            _builder.addThickHorizontalBorder(_row - 1);
            break;
        default:
            throw new std::exception();
    }
}

void TableParser::_parseStyleSchema(std::string schema) {
    CellStyle style = CellStyle::parse(schema);
    _builder.addCellStyle(_row, _col, style);
}
