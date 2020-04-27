pub struct Lexer<'a> {
    input: String,
    chars: std::str::Chars<'a>,
}

impl<'a> Lexer<'a> {
    pub fn new(input: String) -> Lexer<'a> {
        Lexer { input, chars: input.chars() }
    }

    pub fn next_token(&mut self) -> Option<char> {
        return self.chars.next();
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn lex_all(input: &str) -> Vec<String> {
        let lex = Lexer::new(String::from(input));
        let tokens = vec![];
        return tokens;
    }

    #[test]
    fn it_works() {
        assert_eq!(lex_all("=+(){},;"), vec![String::from("=")]);
    }
}
