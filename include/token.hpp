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
    TokenDefinition<TokenType>(TokenType type,
                               const std::string regex,
                               bool discard = false)
        : type(type)
        , regex(std::regex(regex))
        , discard(discard)
    {}

    TokenType type;
    std::regex regex;
    bool discard;
};

template <typename TokenType>
std::ostream& operator<<(std::ostream& os, const Token<TokenType>& token)
{
    os << "Token(type: " << magic_enum::enum_name(token.type) << ", lexeme: '"
       << token.lexeme << "', line: " << token.line
       << ", column: " << token.column << ")";
    return os;
}
