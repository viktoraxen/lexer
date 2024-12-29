use super::pattern::Pattern;
use super::token::Token;
use regex::Regex;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

pub struct Lexer {
    patterns: Vec<Pattern>,
    tokens: Vec<Token>,
    current_line_num: usize,
    current_col_num: usize,
    current_line: String,
}

impl Lexer {
    pub fn new() -> Self {
        Self {
            patterns: Vec::new(),
            tokens: Vec::new(),
            current_line_num: 0,
            current_col_num: 0,
            current_line: String::new(),
        }
    }

    pub fn add_pattern(&mut self, name: &str, pattern: &str) {
        self.patterns
            .push(Pattern::new(name.to_string(), pattern.to_string()));
    }

    pub fn tokenize(&mut self, filename: &str) -> Result<&Vec<Token>, io::Error> {
        let lines = match Lexer::file_lines(filename) {
            Ok(lines) => lines,
            Err(e) => {
                eprintln!("Failed to read content of file {}: {}", filename, e);
                return Err(e);
            }
        };

        for (i, line_result) in lines.enumerate() {
            let line = match line_result {
                Ok(l) => l,
                Err(e) => {
                    eprintln!("Failed to read line!");
                    return Err(e);
                }
            };

            self.current_line_num = i + 1;

            if !self.tokenize_line(&line) {
                eprintln!("Failed to tokenize");
                return Ok(&self.tokens);
            };
        }

        return Ok(&self.tokens);
    }

    fn tokenize_line(&mut self, line: &str) -> bool {
        let mut remaining = line;
        self.current_line = line.to_string();
        self.current_col_num = 1;

        while !remaining.is_empty() {
            let mut found = false;

            for pattern in &self.patterns {
                let format = format!("^{}", pattern.pattern);
                let re = Regex::new(&format).unwrap();

                if let Some(m) = re.find(&remaining) {
                    found = true;

                    let token = Token::new(
                        pattern.name.clone(),
                        m.as_str().to_string(),
                        self.current_line_num,
                        self.current_col_num,
                    );

                    self.tokens.push(token);
                    remaining = &remaining[m.end()..];

                    self.current_col_num += m.end();

                    break;
                }
            }

            if !found {
                eprintln!("Unknown token: {}", remaining);
                self.panic();
                return false;
            }
        }

        return true;
    }

    pub fn print_tokens(&self) {
        let mut line_width = 0;
        let mut col_width = 0;
        let mut content_width = 0;

        for token in &self.tokens {
            line_width = line_width.max(token.line.to_string().len());
            col_width = col_width.max(token.col.to_string().len());
            content_width = content_width.max(token.content.len());
        }

        for token in &self.tokens {
            println!(
                "{:<line_width$}:{:<col_width$} {:<content_width$} ({})",
                token.line,
                token.col,
                token.content,
                token.name,
                line_width = line_width,
                col_width = col_width,
                content_width = content_width
            );
        }
    }

    fn panic(&self) {
        let num_width = self.current_line_num.to_string().len();
        let indicator_offset = num_width + self.current_col_num + 2;

        println!("{}: {}", self.current_line_num, self.current_line);
        println!(
            "{: >indicator_offset$}",
            "^",
            indicator_offset = indicator_offset
        );
        println!(
            "LexerError: Invalid token at line {}, col {}",
            self.current_line_num, self.current_col_num
        );
    }

    fn file_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
    where
        P: AsRef<Path>,
    {
        let file = File::open(filename)?;
        Ok(io::BufReader::new(file).lines())
    }
}
