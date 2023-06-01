#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

#include "token.hpp"
#include "token_type.hpp"

namespace Symbol
{
    const char COL_SEP = '|';
    const char ROW_SEP = '\n';
    const char H_BORDER = '-';
    const char THICK_H_BORDER = '=';
    const char H_MERGE = '<';
    const char V_MERGE = '^';
    const char STYLE = '?';
    const char CODE = '`';
};

class TableLexer
{
    std::string _script;
    int _idx;
    std::vector<Token> _tokens;

public:
    void lex(std::string script);
    std::vector<Token> &getTokens();

private:
    void _lex();
    char _peek();
    char _next();
    void _skipWhitespace();
    void _lexScript();
    void _lexCodeBlock();
    void _lexRow();
    void _lexSeparator();
    void _lexStyle();
    void _lexInlineCode();
    bool _lexBorder(char symbol, TokenType tokenType);
    void _lexBorders();
};

#endif
