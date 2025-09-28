#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <sstream>
#include <vector>

#include "token.hpp"

/**
 * @brief A generic, regular-expression-based lexical analyzer.
 *
 * This class transforms a string of text into a stream of tokens based on a
 * set of provided token definitions. It supports both eager tokenization into a
 * vector and lazy, stream-based tokenization via iterators.
 *
 * @tparam TokenType The enum type used for classifying tokens.
 */
template <typename TokenType>
class Lexer
{
  public:
    /**
     * @brief Constructs an empty, unconfigured lexer.
     */
    Lexer();

    /**
     * @brief Constructs a lexer pre-configured with token definitions.
     *
     * @param definitions A vector of TokenDefinition objects that define the
     * lexer's behavior.
     */
    Lexer(std::vector<TokenDefinition<TokenType>> definitions);
    ~Lexer();

    /**
     * @brief Eagerly tokenizes the entire input string into a vector of tokens.
     *
     * @param input The string to tokenize.
     * @return A vector containing all identified tokens.
     */
    std::vector<Token<TokenType>> tokenize(const std::string& input);

    /**
     * @brief Overload for C-style string literals.
     */
    std::vector<Token<TokenType>> tokenize(const char* input)
    {
        return tokenize(std::string(input));
    }

    /**
     * @brief Eagerly tokenizes the entire content of a file into a vector of
     * tokens.
     *
     * @param filepath The path to the file to tokenize.
     * @return A vector containing all identified tokens.
     */
    std::vector<Token<TokenType>> tokenize_file(const char* filepath)
    {
        std::ifstream file(filepath);

        std::stringstream buffer;
        buffer << file.rdbuf();

        return tokenize(buffer.str());
    }

    /**
     * @brief An input iterator for traversing tokens in a stream.
     *
     * This class allows for lazy evaluation of the input source, producing
     * tokens one by one as it is incremented.
     */
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
        /**
         * @brief Fetches the next token from the lexer.
         */
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

    /**
     * @brief A lightweight wrapper that provides a range for token iteration.
     *
     * This class is designed to be used with a range-based for loop to
     * iterate through the tokens of a source string lazily.
     */
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

    /**
     * @brief Prepares the lexer and returns a TokenStream for the given
     * content.
     *
     * @param content The source string to be tokenized.
     * @return A TokenStream object for lazy iteration.
     */
    TokenStream stream(const std::string& content)
    {
        m_content = content;
        m_contentIt = m_content.cbegin();
        m_current_line_num = 1;
        m_current_col_num = 1;

        return TokenStream(*this);
    }

    /**
     * @brief Prepares the lexer and returns a TokenStream for the given file.
     *
     * @param filepath The path to the source file to be tokenized.
     * @return A TokenStream object for lazy iteration.
     */
    TokenStream stream_from_file(const std::string& filepath)
    {
        std::ifstream file(filepath);

        std::stringstream buffer;
        buffer << file.rdbuf();

        return stream(buffer.str());
    }

  private:
    /// @brief The set of rules for identifying tokens.
    std::vector<TokenDefinition<TokenType>> m_definitions;
    /// @brief The input string being tokenized.
    std::string m_content;
    /// @brief The current line number in the input string.
    size_t m_current_line_num;
    /// @brief The current column number on the current line.
    size_t m_current_col_num;
    /// @brief An iterator pointing to the current position in the input string.
    std::string::const_iterator m_contentIt;

    /**
     * @brief Handles and reports an unexpected token error.
     * @param line_num The line number of the error.
     * @param col_num The column number of the error.
     */
    void unexpected_token(int line_num, int col_num);

    /**
     * @brief Scans the input string for the next valid token.
     * @return An optional containing the next token, or std::nullopt if the
     * end is reached.
     */
    std::optional<Token<TokenType>> next_token();
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
Lexer<TokenType>::Lexer(std::vector<TokenDefinition<TokenType>> definitions)
    : m_content("")
    , m_contentIt(m_content.cbegin())
    , m_definitions(definitions)
    , m_current_col_num(1)
    , m_current_line_num(1)
{}

template <typename TokenType>
Lexer<TokenType>::~Lexer()
{}

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

    for (const auto& token : stream(input))
        tokens.push_back(token);

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
