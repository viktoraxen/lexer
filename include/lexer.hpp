#include <cstddef>
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <vector>

#include "token.hpp"

template <typename TokenType>
class Lexer
{
  public:
    Lexer();
    ~Lexer();

    std::vector<Token<TokenType>> tokenize(const std::string& input);
    std::vector<Token<TokenType>> tokenize(const char* input)
    {
        return tokenize(std::string(input));
    }

    std::optional<Token<TokenType>> next_token();

    void define_token(TokenType type,
                      const std::string& regex,
                      bool discard = false);

    class Iterator
    {
      public:
        using value_t = Token<TokenType>;
        using token_r = const Token<TokenType>&;
        using token_p = const Token<TokenType>*;
        using it_cat = std::input_iterator_tag;
        using diff_t = std::ptrdiff_t;

        Iterator(Lexer* lexer)
            : m_lexer(lexer)
        {
            advance();
        }

        Iterator()
            : m_lexer(nullptr)
        {}

        token_r operator*() const { return *m_current; }
        token_p operator->() const { return &(*m_current); }

        Iterator& operator++()
        {
            advance();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const
        {
            return (m_lexer == other.m_lexer) &&
                   (!m_lexer || (!m_current && !other.m_current));
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

      private:
        void advance()
        {
            if (m_lexer)
            {
                m_current = m_lexer->next_token();

                if (!m_current)
                    m_lexer = nullptr;
            }
        }

        Lexer* m_lexer;
        std::optional<value_t> m_current;
    };

    class TokenStream
    {
      public:
        TokenStream(Lexer& lexer)
            : m_lexer(lexer)
        {}

        Iterator begin() { return Iterator(&m_lexer); }
        Iterator end() { return Iterator(); }

      private:
        Lexer& m_lexer;
    };

    TokenStream token_stream(const std::string& content)
    {
        m_content = content;
        m_contentIt = m_content.cbegin();
        m_current_line_num = 1;
        m_current_col_num = 1;

        return TokenStream(*this);
    }

  private:
    std::vector<TokenDefinition<TokenType>> m_definitions;
    std::string m_content;
    size_t m_current_line_num;
    size_t m_current_col_num;
    std::string::const_iterator m_contentIt;

    void unexpected_token(int line_num, int col_num);
};

template <typename TokenType>
Lexer<TokenType>::Lexer()
    : m_content("")
    , m_contentIt(m_content.cbegin())
    , m_definitions()
    , m_current_col_num(1)
    , m_current_line_num(1)
{}
template <typename TokenType>
Lexer<TokenType>::~Lexer()
{}

template <typename TokenType>
void Lexer<TokenType>::define_token(const TokenType type,
                                    const std::string& regex,
                                    bool discard)
{
    m_definitions.push_back({
        .type = type,
        .regex = std::regex(regex),
        .discard = discard,
    });
}

template <typename TokenType>
std::optional<Token<TokenType>> Lexer<TokenType>::next_token()
{
    if (m_contentIt >= m_content.cend())
        return std::nullopt;

    std::smatch bestMatch;
    const TokenDefinition<TokenType>* bestDefinition = nullptr;

    for (const auto& definition : m_definitions)
    {
        std::smatch match;
        auto regex = definition.regex;

        if (!std::regex_search(m_contentIt, m_content.cend(), match, regex,
                               std::regex_constants::match_continuous))
            continue;

        if (!bestDefinition || match.length() > bestMatch.length())
        {
            bestMatch = match;
            bestDefinition = &definition;
        }
    }

    if (bestMatch.empty())
    {
        unexpected_token(m_current_line_num, m_current_col_num);
        return std::nullopt;
    }

    auto lexeme = bestMatch.str();

    for (const auto& c : lexeme)
    {
        if (c == '\n')
        {
            m_current_line_num++;
            m_current_col_num = 1;
        }
        else
            m_current_col_num++;
    }

    m_contentIt += lexeme.size();

    if (bestDefinition->discard)
        return next_token();

    Token<TokenType> token = {
        .type = bestDefinition->type,
        .lexeme = lexeme,
        .line = m_current_line_num,
        .column = m_current_col_num,
    };

    return token;
}

template <typename TokenType>
std::vector<Token<TokenType>>
Lexer<TokenType>::tokenize(const std::string& input)
{
    std::vector<Token<TokenType>> tokens;

    // Use a stringstrem to iterate the content line by line
    std::istringstream input_stream(input);

    // Track the line and column number for error messages
    std::string line;
    int line_num = 1;

    while (std::getline(input_stream, line))
    {
        // Used to iterate a line
        std::string::const_iterator contentIt = line.cbegin();

        int col_num = 1;

        while (contentIt != line.cend())
        {
            std::smatch bestMatch;
            const TokenDefinition<TokenType>* bestDefinition = nullptr;

            for (const auto& definition : m_definitions)
            {
                std::smatch match;
                auto regex = definition.regex;

                if (!std::regex_search(contentIt, line.cend(), match, regex,
                                       std::regex_constants::match_continuous))
                    continue;

                if (!bestDefinition || match.length() > bestMatch.length())
                {
                    bestMatch = match;
                    bestDefinition = &definition;
                }
            }

            if (bestMatch.empty())
            {
                unexpected_token(line_num, col_num);
                return {};
            }

            auto lexeme = bestMatch.str();

            contentIt += lexeme.size();
            col_num += lexeme.size();

            if (bestDefinition->discard)
                continue;

            tokens.push_back({
                .type = bestDefinition->type,
                .lexeme = lexeme,
                .line = 0,
                .column = 0,
            });
        }

        line_num++;
    }

    return tokens;
}

template <typename TokenType>
void Lexer<TokenType>::unexpected_token(int line_num, int col_num)
{
    std::string line;

    // Use a stringstrem to iterate the content line by line
    std::istringstream input_stream(m_content);

    for (int i = 0; i < line_num; i++)
        std::getline(input_stream, line);

    std::cerr << line << std::endl;
    std::cerr << std::string(col_num - 1, ' ') << "^" << std::endl;
    std::cerr << "Unexpected token at line " << line_num << " and column "
              << col_num << std::endl;
}
