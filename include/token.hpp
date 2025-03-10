#include <regex>
#include <string>

enum class TokenConsumeMode
{
    Consume,
    Discard,
};

template <typename TokenType> struct Token
{
    Token() = default;
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type)
        , lexeme(lexeme)
        , line(line)
        , column(column)
    {
    }
    TokenType type;
    std::string lexeme;
    int line, column;
};

template <typename TokenType> struct TokenDefinition
{
    TokenDefinition(TokenType type,
                    const std::string& regex,
                    TokenConsumeMode consume_mode)
        : type(type)
        , regex(regex)
        , consume_mode(consume_mode)
    {
    }
    TokenType type;
    std::regex regex;
    TokenConsumeMode consume_mode;
};
