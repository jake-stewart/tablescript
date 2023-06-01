#ifndef PARSER_HPP
#define PARSER_HPP

#include "cell_content.hpp"
#include "cell_style.hpp"
#include "table.hpp"
#include "table_builder.hpp"
#include "token.hpp"

class TableParser
{
    TableBuilder &_builder;
    std::vector<Token> *_tokens;
    int _row;
    int _col;
    int _idx;

public:
    TableParser(TableBuilder &builder) : _builder(builder) {}
    void parse(std::vector<Token> *tokens);

private:
    const Token *_peek();
    const Token *_next();
    void _parse();
    void _parseToken(const Token *token);
    void _parseStyleSchema(std::string schema);
};

#endif
