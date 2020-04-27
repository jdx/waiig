use crate::token::{Token, TokenType};

pub struct Lexer<'a> {
    chars: std::str::Chars<'a>,
}

impl<'a> Lexer<'a> {
    pub fn new(input: &'a String) -> Lexer<'a> {
        Lexer { chars: input.chars() }
    }

    pub fn next_token(&mut self) -> Option<Token> {
        let c = match self.chars.next() {
            Some(c) => c,
            None => return None,
        };
        let mut type_: Option<TokenType> = None;

        match type_ {
            Some(t) => return Some(Token {
                type_: t,
                literal: c.to_string(),
            }),
            _ => {},
        }

        return Some(Token{
            type_: t,
            literal: c.to_string(),
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::token::TokenType::ILLEGAL;

    fn lex_all(input: &str) -> Vec<Token> {
        let s = String::from(input);
        let mut lex = Lexer::new(&s);
        let mut tokens = vec![];
        loop {
            match lex.next_token() {
                Some(tok) => tokens.push(tok),
                None => break,
            }
        }
        return tokens;
    }

    #[test]
    fn it_works() {
        assert_eq!(lex_all("
func TestNextToken(t *testing.T) {
    input := `let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
"), vec![Token { type_: ILLEGAL, literal: String::from("X") }]);
    }
}
