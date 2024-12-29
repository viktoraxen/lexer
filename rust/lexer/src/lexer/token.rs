pub struct Token {
    pub name: String,
    pub content: String,
    pub line: usize,
    pub col: usize,
}

impl Token {
    pub fn new(name: String, content: String, line: usize, col: usize) -> Self {
        Self {
            name,
            content,
            line,
            col,
        }
    }
}
