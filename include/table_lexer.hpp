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
    char _peek(int n);
    char _next();
    char _next(int n);
    void _skipWhitespace();
    void _lexRow();
    void _lexSeparator();
    void _lexStyle();
    bool _lexBorder(char symbol, TokenType tokenType);
    void _lexBorders();

    void _lexLua(std::string &script);
    void _lexLuaString(std::string &script, char type);
    void _lexLuaMultilineString(std::string &script);
    void _lexLuaTable(std::string &script);
};

#endif
