#include <fstream>
#include <sstream>

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
    Lexer<TestTokenType> lexer;

    lexer.define_token(TestTokenType::Whitespace, "\\s+", true);

    lexer.define_token(TestTokenType::Ampersand, "&");
    lexer.define_token(TestTokenType::Asterisk, "\\*");
    lexer.define_token(TestTokenType::BackwardSlash, "\\\\");
    lexer.define_token(TestTokenType::CharType, "char");
    lexer.define_token(TestTokenType::Colon, ":");
    lexer.define_token(TestTokenType::Comma, ",");
    lexer.define_token(TestTokenType::Dot, "\\.");
    lexer.define_token(TestTokenType::DoubleColon, "::");
    lexer.define_token(TestTokenType::Equal, "=");
    lexer.define_token(TestTokenType::Exclamation, "!");
    lexer.define_token(TestTokenType::Float, "[0-9]+\\.[0-9]+");
    lexer.define_token(TestTokenType::FloatType, "float");
    lexer.define_token(TestTokenType::ForwardSlash, "/");
    lexer.define_token(TestTokenType::Identifier, "[a-zA-Z_][a-zA-Z0-9_]*");
    lexer.define_token(TestTokenType::Include, "#include");
    lexer.define_token(TestTokenType::Int, "[0-9]+");
    lexer.define_token(TestTokenType::IntegerType, "int");
    lexer.define_token(TestTokenType::LeftAngle, "<");
    lexer.define_token(TestTokenType::LeftBrace, "\\{");
    lexer.define_token(TestTokenType::LeftBracket, "\\[");
    lexer.define_token(TestTokenType::LeftParenthesis, "\\(");
    lexer.define_token(TestTokenType::LeftStream, "<<");
    lexer.define_token(TestTokenType::Plus, "\\+");
    lexer.define_token(TestTokenType::Quote, "\"");
    lexer.define_token(TestTokenType::RightAngle, ">");
    lexer.define_token(TestTokenType::RightBrace, "\\}");
    lexer.define_token(TestTokenType::RightBracket, "\\]");
    lexer.define_token(TestTokenType::RightParenthesis, "\\)");
    lexer.define_token(TestTokenType::RightStream, ">>");
    lexer.define_token(TestTokenType::Semicolon, ";");
    lexer.define_token(TestTokenType::SingleQuote, "\'");
    lexer.define_token(TestTokenType::StringType, "string");

    std::ifstream file("res/input.txt");

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::cout << buffer.str() << std::endl;

    for (const auto& token : lexer.token_stream(buffer.str()))
    {
        std::cout << token << std::endl;
    }

    return 0;
}
