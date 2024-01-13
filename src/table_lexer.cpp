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
    // while (true) {
    //     _skipWhitespace();
    //     char c;
    //     if ((c = _peek()) == 0 || c != Symbol::ROW_SEP) {
    //         break;
    //     }
    //     _next();
    // }
    while (true) {
        if (_peek() == '{') {
            std::string script;
            _next();
            _lexLua(script);
            _next();
            if (script.find('\n') != std::string::npos) {
                _tokens.push_back(Token(TOKEN_CODE_BLOCK, script));
                if (_peek() == '\n') {
                    _next();
                    continue;
                }
                continue;
            }
            _tokens.push_back(Token(TOKEN_INLINE_CODE, script));
        }
        else {
            _lexBorders();
        }
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
    return _peek(0);
}

char TableLexer::_peek(int n) {
    if (_idx + n >= _script.length()) {
        return 0;
    }
    return _script[_idx + n];
}

char TableLexer::_next() {
    return _next(1);
}

char TableLexer::_next(int n) {
    _idx += n;
    if (_idx > _script.length()) {
        return 0;
    }
    return _script[_idx - n];
}

void TableLexer::_skipWhitespace() {
    while (_peek() && strchr(" \t", _peek())) {
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
        else if (c == '{') {
            if (text.length()) {
                _tokens.push_back(Token(TOKEN_TEXT, text));
                text = "";
            }
            _next();
            std::string script;
            _lexLua(script);
            _tokens.push_back(Token(TOKEN_INLINE_CODE, script));
            _next();
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

void TableLexer::_lexLua(std::string &script) {
    while (true) {
        char c = _peek();
        if (c == 0 || c == '}') {
            break;
        }
        script += _next();
        switch (c) {
            case '{':
                _lexLua(script);
                if (_peek() == '}') {
                    script += _next();
                }
                break;
            case '"':
            case '\'':
                _lexLuaString(script, c);
                if (_peek() == c) {
                    script += _next();
                }
                break;
            case '[':
                if (_peek() == '[') {
                    script += _next();
                    _lexLuaMultilineString(script);
                    if (_peek() == ']') {
                        script += _next();
                        if (_peek() == ']') {
                            script += _next();
                        }
                    }
                    break;
                }
        }
        if (c == 0) {
            break;
        }
    }
}

void TableLexer::_lexLuaMultilineString(std::string &script) {
    while (true) {
        char c = _peek();
        if (c == 0) {
            return;
        }
        if (c == ']') {
            if (_peek(1) == ']') {
                return;
            }
        }
        script += _next();
    }
}

void TableLexer::_lexLuaString(std::string &script, char type) {
    while (true) {
        char c = _peek();
        if (c == 0) {
            return;
        }
        if (c == type) {
            return;
        }
        script += _next();
        if (c == '\\' && _peek()) {
            script += _next();
        }
    }
}
