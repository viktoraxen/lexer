mod lexer;

fn main() {
    let mut lexer = lexer::Lexer::new();
    lexer.add_pattern("Whitespace", "\\s+");
    lexer.add_pattern("Keyword", "from");
    lexer.add_pattern("Keyword", "import");
    lexer.add_pattern("Quote", "\"");
    lexer.add_pattern("Dot", "\\.");
    lexer.add_pattern("Comma", ",");
    lexer.add_pattern("LParen", "\\(");
    lexer.add_pattern("RParen", "\\)");
    lexer.add_pattern("Equal", "=");
    lexer.add_pattern("Plus", "\\+");
    lexer.add_pattern("Digit", "\\d+");
    lexer.add_pattern("Backslash", "\\\\");
    lexer.add_pattern("Identifier", "\\w+");

    let tokens = lexer.tokenize("../../python/main.py");
    lexer.print_tokens();
}
