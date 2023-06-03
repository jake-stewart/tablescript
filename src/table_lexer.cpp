#include "../include/table_lexer.hpp"
#include <cstring>

void TableLexer::lex(std::string script) {
    _script = script;
    _idx = 0;
    _tokens.clear();
    _lex();
}

std::vector<Token> &TableLexer::getTokens() {
    return _tokens;
}

void TableLexer::_lex() {
    _lexScript();
    while (true) {
        _skipWhitespace();
        char c;
        if ((c = _peek()) == 0 || c != Symbol::ROW_SEP) {
            break;
        }
        _next();
    }
    while (true) {
        _lexBorders();
        if (_peek() == Symbol::STYLE) {
            _lexStyle();
        }
        else if (_peek() == Symbol::V_MERGE) {
            _tokens.push_back(Token(TOKEN_V_MERGE));
            _next();
        }
        _lexRow();
        _next();
        if (_peek() == 0) {
            break;
        }
        _tokens.push_back(Token(TOKEN_ROW_SEP));
        _skipWhitespace();
    }
}

char TableLexer::_peek() {
    if (_idx >= _script.length()) {
        return 0;
    }
    return _script[_idx];
}

char TableLexer::_next() {
    _idx += 1;
    if (_idx > _script.length()) {
        return 0;
    }
    return _script[_idx - 1];
}

void TableLexer::_skipWhitespace() {
    while (_peek() && strchr(" \t", _peek())) {
        _next();
    }
}

void TableLexer::_lexScript() {
    char c;
    if (_peek() != Symbol::CODE) {
        return;
    }
    int start = _idx;
    _next();
    if (_next() == Symbol::CODE && _next() == Symbol::CODE) {
        _lexCodeBlock();
        return;
    }
    _idx = start;
}

void TableLexer::_lexCodeBlock() {
    std::string code = "";
    char c;
    while (true) {
        while ((c = _peek()) != 0 && c != Symbol::CODE) {
            _next();
            code += c;
        }
        if (_peek() == 0) {
            break;
        }
        int idx = _idx;
        _next();
        if (_next() == Symbol::CODE && _next() == Symbol::CODE) {
            break;
        }
        _idx = idx;
        code += _next();
    }
    _next();
    _tokens.push_back(Token(TOKEN_CODE_BLOCK, code));
    while (_peek() == Symbol::ROW_SEP) {
        _next();
    }
}

void TableLexer::_lexRow() {
    char c;
    std::string text;
    while ((c = _peek()) && c != Symbol::ROW_SEP) {
        if (c == Symbol::COL_SEP) {
            if (text.length()) {
                _tokens.push_back(Token(TOKEN_TEXT, text));
                text = "";
            }
            _lexSeparator();
        }
        else if (c == Symbol::CODE) {
            if (text.length()) {
                _tokens.push_back(Token(TOKEN_TEXT, text));
                text = "";
            }
            _lexInlineCode();
        }
        else if (c == '\\') {
            _next();
            c = _next();
            if (c == 'n') {
                text += '\n';
            }
            else if (c != 0) {
                text += c;
            }
        }
        else {
            text += c;
            _next();
        }
    }
    if (text.length()) {
        _tokens.push_back(Token(TOKEN_TEXT, text));
    }
}

void TableLexer::_lexSeparator() {
    _tokens.push_back(Token(TOKEN_COL_SEP));
    _next();
    _skipWhitespace();
    if (_peek() == Symbol::H_MERGE) {
        _tokens.push_back(Token(TOKEN_H_MERGE));
        _next();
    }
    else if (_peek() == Symbol::V_MERGE) {
        _tokens.push_back(Token(TOKEN_V_MERGE));
        _next();
    }
    else if (_peek() == Symbol::STYLE) {
        _lexStyle();
    }
    _skipWhitespace();
}

void TableLexer::_lexStyle() {
    _next();
    std::string style = "";
    char c;
    while ((c = _peek()) != 0 && isalpha(c)) {
        style += c;
        _next();
    }
    _tokens.push_back(Token(TOKEN_STYLE, style));
}

void TableLexer::_lexInlineCode() {
    _next();
    std::string code = "";
    char c;
    while ((c = _peek()) != 0 && c != Symbol::CODE && c != Symbol::ROW_SEP) {
        code += c;
        _next();
    }
    _tokens.push_back(Token(TOKEN_INLINE_CODE, code));
    if (_peek() == Symbol::CODE) {
        _next();
    }
}

bool TableLexer::_lexBorder(char symbol, TokenType tokenType) {
    if (_idx >= _script.length()) {
        return false;
    }
    int idx = _idx;
    while (idx < _script.length()) {
        if (_script[idx] == symbol) {
            idx += 1;
            continue;
        }
        if (_script[idx] == Symbol::ROW_SEP) {
            break;
        }
        return false;
    }
    _tokens.push_back(Token(tokenType));
    _idx = idx + 1;
    return true;
}

void TableLexer::_lexBorders() {
    while (true) {
        bool hasBorder = false;
        if (_peek() == Symbol::H_BORDER) {
            hasBorder = _lexBorder(Symbol::H_BORDER, TOKEN_H_BORDER);
            if (hasBorder) {
                continue;
            }
        }
        if (_peek() == Symbol::THICK_H_BORDER) {
            hasBorder = _lexBorder(
                Symbol::THICK_H_BORDER, TOKEN_THICK_H_BORDER
            );
        }
        if (!hasBorder) {
            break;
        }
    }
}
