import re, math


class Lexer:
    class Token:
        def __init__(self, name: str, pattern: str):
            self.name = name
            self.pattern = pattern

    class TokenMatch:
        def __init__(self, token: "Lexer.Token", value: str, line: int, col: int):
            self.token = token
            self.value = value
            self.line = line
            self.col = col

        def __iter__(self):
            return iter((self.token, self.value))

    def __init__(self):
        self.tokens = []
        self.sequence = []
        self.current_text = ""
        self.current_line = 0
        self.current_col = 0
        self.line_rem = ""

    def add_token(self, name: str, pattern: str) -> None:
        """
        Add a token for the lexer to search for when tokenizing. Tokens added
        earlier are matcher earlier.
        """
        self.tokens.append(Lexer.Token(name, pattern))

    def tokenize(self, text: str) -> list["Lexer.TokenMatch"]:
        """
        Tokenize the given text and return a list of TokenMatch objects. If an
        invalid token is found, an empty list is returned.
        """
        self.current_text = text

        for i, line in enumerate(self.current_text.splitlines()):
            self.current_line = i + 1
            self.current_col = 1

            self.line_rem = line

            while self.line_rem:
                for token in self.tokens:
                    pattern = r"^" + token.pattern

                    match = re.match(pattern, self.line_rem)

                    if match:
                        self.line_rem = self.line_rem[match.end() :]
                        token_match = Lexer.TokenMatch(
                            token, match.group(), self.current_line, self.current_col
                        )
                        self.sequence.append(token_match)
                        self.current_col += match.end()
                        break

                if match is None:
                    return self.__fail()

        return self.sequence

    def print_sequence(self) -> None:
        line_width = max([len(str(token_match.line)) for token_match in self.sequence])
        col_width = max([len(str(token_match.col)) for token_match in self.sequence])
        token_width = max([len(token_match.token.name) for token_match in self.sequence])

        for token_match in self.sequence:
            l = token_match.line
            c = token_match.col
            t = token_match.token.name
            v = token_match.value

            print(f"{l: <{line_width}}:{c: <{col_width}} {v: <{token_width}} ({t})")

    def __fail(self):
        lineNumWidth = int(math.log10(self.current_line)) + 1
        indicator_offset = self.current_col + lineNumWidth + 1

        print(f"{self.current_line}: {self.current_text.splitlines()[self.current_line - 1]}")
        print(" " * indicator_offset + "^")
        print(f"LexerError: Invalid token at line {self.current_line}, col {self.current_line}")

        return []
