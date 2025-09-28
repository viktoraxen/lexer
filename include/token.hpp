#pragma once

#include <ostream>
#include <regex>
#include <string>

#include "magic_enum/magic_enum.hpp"

template <typename TokenType>
struct Token
{
    TokenType type;
    std::string lexeme;
    int line, column;
};

template <typename TokenType>
struct TokenDefinition
{
    TokenType type;
    std::regex regex;
    bool discard = false;
};

template <typename TokenType>
std::ostream& operator<<(std::ostream& os, const Token<TokenType>& token)
{
    os << "Token(type: " << magic_enum::enum_name(token.type) << ", lexeme: '"
       << token.lexeme << "', line: " << token.line
       << ", column: " << token.column << ")";
    return os;
}
