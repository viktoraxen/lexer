#pragma once

#include <ostream>
#include <regex>
#include <string>

#include "magic_enum/magic_enum.hpp"

/**
 * @brief Represents a token identified by the lexer.
 *
 * This struct holds information about a single token that has been processed
 * from the input string. It includes the token's type, the actual text (lexeme)
 * that was matched, and its position in the source file.
 *
 * @tparam TokenType The enum type used for classifying tokens.
 */
template <typename TokenType>
struct Token
{
    /// @brief The type of the token from the TokenType enum.
    TokenType type;
    /// @brief The substring from the input that matches the token's pattern.
    std::string lexeme;
    /// @brief The line number in the source where the token appears.
    int line;
    /// @brief The column number in the source where the token begins.
    int column;
};

/**
 * @brief Defines the properties of a token type for the lexer.
 *
 * This struct is used to configure the lexer. Each instance defines a rule
 * for matching a specific type of token, including the regular expression
 * to match and whether the token should be kept or discarded (e.g., whitespace).
 *
 * @tparam TokenType The enum type used for classifying tokens.
 */
template <typename TokenType>
struct TokenDefinition
{
    /**
     * @brief Constructs a new TokenDefinition.
     *
     * @param type The token type this definition corresponds to.
     * @param regex The regular expression string used to match this token.
     * @param discard If true, tokens of this type will be ignored by the lexer. Defaults to false.
     */
    TokenDefinition<TokenType>(TokenType type,
                               const std::string regex,
                               bool discard = false)
        : type(type)
        , regex(std::regex(regex))
        , discard(discard)
    {}

    /// @brief The type of the token this definition creates.
    TokenType type;
    /// @brief The compiled regular expression for matching.
    std::regex regex;
    /// @brief A flag indicating if matched tokens should be discarded.
    bool discard;
};

/**
 * @brief Overloads the stream insertion operator for easy printing of Tokens.
 *
 * This allows a Token object to be written directly to an output stream
 * (e.g., std::cout), formatting it in a human-readable way for debugging.
 *
 * @tparam TokenType The enum type used for classifying tokens.
 * @param os The output stream to write to.
 * @param token The Token object to output.
 * @return A reference to the output stream.
 */
template <typename TokenType>
std::ostream& operator<<(std::ostream& os, const Token<TokenType>& token)
{
    os << "Token(type: " << magic_enum::enum_name(token.type) << ", lexeme: '"
       << token.lexeme << "', line: " << token.line
       << ", column: " << token.column << ")";
    return os;
}