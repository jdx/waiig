pub struct Lexer {
    input: String,
}

impl Lexer {
    pub fn new(input: String) -> Lexer {
        Lexer { input }
    }

    pub fn next_token(&self) -> String {
        return String::from("x");
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let lex = Lexer::new(String::from("foo"));
        assert_eq!(lex.next_token(), "x");
    }
}
