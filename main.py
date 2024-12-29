from lexer import Lexer

text = open("main.py").read()

lexer = Lexer()
lexer.add_token("Whitespace", r"\s+")
lexer.add_token("Keyword", r"from")
lexer.add_token("Keyword", r"import")
lexer.add_token("Digit", r"\d+")
lexer.add_token("Identifier", r"\w+")
lexer.add_token("Quote", r"\"")
lexer.add_token("Dot", r"\.")
lexer.add_token("Comma", r",")
lexer.add_token("LParen", r"\(")
lexer.add_token("RParen", r"\)")
lexer.add_token("BackSlash", r"\\")
lexer.add_token("Equal", r"=")
lexer.add_token("Plus", r"\+")

lexer.tokenize(text)
lexer.print_sequence()
