#include <fstream>
#include <sstream>
#include <vector>

#include "lexer.hpp"

enum class TestTokenType
{
    Ampersand,
    Asterisk,
    BackwardSlash,
    CharType,
    Colon,
    Comma,
    Dot,
    DoubleColon,
    Equal,
    Exclamation,
    Float,
    FloatType,
    ForwardSlash,
    Identifier,
    Include,
    IncludePathBracket,
    IncludePathQuote,
    Int,
    IntegerType,
    LeftAngle,
    LeftBrace,
    LeftBracket,
    LeftParenthesis,
    LeftStream,
    Plus,
    Quote,
    RightAngle,
    RightBrace,
    RightBracket,
    RightParenthesis,
    RightStream,
    Semicolon,
    SingleQuote,
    StringType,
    Whitespace,
};

int main()
{
    std::vector<TokenDefinition<TestTokenType>> definitions = {
        {TestTokenType::Whitespace, "\\s+", true},
        {TestTokenType::Ampersand, "&"},
        {TestTokenType::Asterisk, "\\*"},
        {TestTokenType::BackwardSlash, "\\\\"},
        {TestTokenType::CharType, "char"},
        {TestTokenType::Colon, ":"},
        {TestTokenType::Comma, ","},
        {TestTokenType::Dot, "\\."},
        {TestTokenType::DoubleColon, "::"},
        {TestTokenType::Equal, "="},
        {TestTokenType::Exclamation, "!"},
        {TestTokenType::Float, "[0-9]+\\.[0-9]+"},
        {TestTokenType::FloatType, "float"},
        {TestTokenType::ForwardSlash, "/"},
        {TestTokenType::Identifier, "[a-zA-Z_][a-zA-Z0-9_]*"},
        {TestTokenType::Include, "#include"},
        {TestTokenType::Int, "[0-9]+"},
        {TestTokenType::IntegerType, "int"},
        {TestTokenType::LeftAngle, "<"},
        {TestTokenType::LeftBrace, "\\{"},
        {TestTokenType::LeftBracket, "\\["},
        {TestTokenType::LeftParenthesis, "\\("},
        {TestTokenType::LeftStream, "<<"},
        {TestTokenType::Plus, "\\+"},
        {TestTokenType::Quote, "\""},
        {TestTokenType::RightAngle, ">"},
        {TestTokenType::RightBrace, "\\}"},
        {TestTokenType::RightBracket, "\\]"},
        {TestTokenType::RightParenthesis, "\\)"},
        {TestTokenType::RightStream, ">>"},
        {TestTokenType::Semicolon, ";"},
        {TestTokenType::SingleQuote, "\'"},
        {TestTokenType::StringType, "string"},
    };

    Lexer<TestTokenType> lexer(definitions);

    for (const auto& token : lexer.stream_from_file("res/input.txt"))
        std::cout << token << std::endl;

    auto tokens = lexer.tokenize_file("res/input.txt");

    for (const auto& token : tokens)
        std::cout << token << std::endl;

    return 0;
}
