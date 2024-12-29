pub struct Pattern {
    pub name: String,
    pub pattern: String,
}

impl Pattern {
    pub fn new(name: String, pattern: String) -> Self {
        Self { name, pattern }
    }
}

