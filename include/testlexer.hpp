#include "lexer.hpp"

enum class TestTokenType
{
    IntegerType,
    FloatType,
    CharType,
    StringType,
    Int,
    Float,
    Identifier,
    Whitespace,
    Include,
    IncludePathBracket,
    IncludePathQuote,
    LeftParenthesis,
    RightParenthesis,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    LeftAngle,
    RightAngle,
    Semicolon,
    Comma,
    Dot,
    Colon,
    DoubleColon,
    LeftStream,
    RightStream,
    Ampersand,
    Asterisk,
    Quote,
    Exclamation,
    Plus,
    Equal,
};

class TestLexer : public Lexer<TestTokenType>
{
  public:
    TestLexer()
    {
        define_token(TestTokenType::Whitespace, "\\s+",
                     TokenConsumeMode::Discard);

        define_token(TestTokenType::IntegerType, "int");
        define_token(TestTokenType::FloatType, "float");
        define_token(TestTokenType::CharType, "char");
        define_token(TestTokenType::StringType, "string");
        define_token(TestTokenType::Include, "#include");
        define_token(TestTokenType::LeftBrace, "\\{");
        define_token(TestTokenType::RightBrace, "\\}");
        define_token(TestTokenType::LeftBracket, "\\[");
        define_token(TestTokenType::RightBracket, "\\]");
        define_token(TestTokenType::LeftParenthesis, "\\(");
        define_token(TestTokenType::RightParenthesis, "\\)");
        define_token(TestTokenType::LeftAngle, "<");
        define_token(TestTokenType::RightAngle, ">");
        define_token(TestTokenType::LeftStream, "<<");
        define_token(TestTokenType::RightStream, ">>");
        define_token(TestTokenType::Semicolon, ";");
        define_token(TestTokenType::Comma, ",");
        define_token(TestTokenType::Dot, "\\.");
        define_token(TestTokenType::Colon, ":");
        define_token(TestTokenType::DoubleColon, "::");
        define_token(TestTokenType::Ampersand, "&");
        define_token(TestTokenType::Exclamation, "!");
        define_token(TestTokenType::Plus, "\\+");
        define_token(TestTokenType::Equal, "=");
        define_token(TestTokenType::Asterisk, "\\*");
        define_token(TestTokenType::Quote, "\"");
        define_token(TestTokenType::Int, "[0-9]+");
        define_token(TestTokenType::Float, "[0-9]+\\.[0-9]+");
        define_token(TestTokenType::Identifier, "[a-zA-Z_][a-zA-Z0-9_]*");
    };
};
