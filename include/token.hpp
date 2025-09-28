#pragma once

#include <ostream>
#include <regex>
#include <string>

#include "magic_enum/magic_enum.hpp"

enum class TokenConsumeMode
{
    Consume,
    Discard,
};

template <typename TokenType>
struct Token
{
    Token() = default;
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type)
        , lexeme(lexeme)
        , line(line)
        , column(column)
    {}
    TokenType type;
    std::string lexeme;
    int line, column;
};

template <typename TokenType>
std::ostream& operator<<(std::ostream& os, const Token<TokenType>& token)
{
    os << "Token(type: " << magic_enum::enum_name(token.type) << ", lexeme: '"
       << token.lexeme << "', line: " << token.line
       << ", column: " << token.column << ")";
    return os;
}

template <typename TokenType>
struct TokenDefinition
{
    TokenDefinition(TokenType type,
                    const std::string& regex,
                    bool discard = false)
        : type(type)
        , regex(regex)
        , discard(discard)
    {}
    TokenType type;
    std::regex regex;
    bool discard;
};
