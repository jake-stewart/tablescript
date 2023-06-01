#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_type.hpp"
#include <string>

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType type);
    Token(TokenType type, std::string value);
};

#endif
