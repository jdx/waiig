use crate::token::{Token, TokenType, lookup_ident};
use std::iter::{Peekable, FromIterator};
use std::str::Chars;

pub struct Lexer<'a> {
    chars: Peekable<Chars<'a>>,
    cur: Vec<char>,
    row: usize,
    col: usize,
}

impl<'a> Lexer<'a> {
    pub fn new(input: &'a String) -> Lexer<'a> {
        Lexer {
            chars: input.chars().peekable(),
            cur: vec![],
            row: 0,
            col: 1,
        }
    }

    fn eat_whitespace(&mut self) {
        while match self.peek() {
            Some(c) if c.is_whitespace() => true,
            _ => false,
        } {
            self.next_char();
        }
        self.cur = vec![];
    }

    fn peek(&mut self) -> Option<char> {
        self.chars.peek().copied()
    }

    fn next_char(&mut self) -> Option<char> {
        match self.chars.next() {
            Some(c) => {
                match c {
                    '\n' => {
                        self.col = 1;
                        self.row += 1;
                    },
                    _ => {
                        self.col += 1;
                    }
                }
                self.cur.push(c);
                Some(c)
            }
            None => None,
        }
    }

    fn read_equals(&mut self) -> TokenType {
        match self.peek() {
            Some('=') => {
                self.next_char();
                TokenType::Eq
            }
            _ => TokenType::Assign,
        }
    }

    fn read_identifier(&mut self) -> TokenType {
        while match self.peek() {
            Some(c) if c.is_alphanumeric() || c == '_' => true,
            _ => false,
        } { self.next_char(); }

        lookup_ident(&String::from_iter(&self.cur))
    }

    fn read_int(&mut self) -> TokenType {
        while match self.peek() {
            Some(c) if c.is_numeric() => true,
            _ => false,
        } { self.next_char(); }

        TokenType::Int
    }

    fn read_bang(&mut self) -> TokenType {
        match self.peek() {
            Some('=') => {
                self.next_char();
                TokenType::NotEq
            }
            _ => TokenType::Bang,
        }
    }

    fn read_string(&mut self) -> TokenType {
        let mut prev: Option<char> = None;
        while match (self.peek(), prev) {
            // escaped quote
            (Some('"'), Some('\\')) => true,

            // terminated string
            (Some('"'), _) => {
                self.next_char();
                false
            }

            // EOF
            (None, _) => {
                // self.emit_error(Error::from_token(
                //     &self.token,
                //     String::from("x")));
                return TokenType::Illegal;
            }

            // still inside string
            _ => true,
        } {
            prev = self.next_char();
        }

        TokenType::Str
    }

    // fn emit_error(&self, error: Error) {
    //     (*self.on_error)(error);
    // }
}

impl<'a> Iterator for Lexer<'a> {
    type Item = Token;

    fn next(&mut self) -> Option<Token> {
        use TokenType::*;

        self.eat_whitespace();

        let mut tok = Token::new();
        tok.row = self.row;
        tok.col = self.col;

        tok.type_ = match self.next_char() {
            Some(c) => match c {
                ';' => Semicolon,
                '(' => LParen,
                ')' => RParen,
                ',' => Comma,
                '+' => Plus,
                '{' => LBrace,
                '}' => RBrace,
                '-' => Minus,
                '/' => Slash,
                '*' => Asterisk,
                '<' => LT,
                '>' => GT,
                '[' => LBracket,
                ']' => RBracket,
                ':' => Colon,
                '!' => self.read_bang(),
                '=' => self.read_equals(),
                '"' => self.read_string(),
                c if c.is_alphabetic() => self.read_identifier(),
                c if c.is_numeric() => self.read_int(),
                _ => Illegal,
            },
            None => return None,
        };

        tok.literal = String::from_iter(&self.cur);

        Some(tok)
    }
}

// #[derive(Debug)]
// pub struct Error {
//     row: usize,
//     col: usize,
//     message: String,
// }
//
// impl fmt::Display for Error {
//     fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
//         write!(f, "error lexing [{}:{}] {}", self.row, self.col, self.message)
//     }
// }
//
// impl error::Error for Error {}
//
// impl Error {
//     fn from_token(token: &Token, message: String) -> Error {
//         Error {
//             message,
//             row: token.row,
//             col: token.col,
//         }
//     }
// }

#[cfg(test)]
mod tests {
    use super::*;
    use pretty_assertions::{assert_eq};
    use TokenType::*;

    #[test]
    fn lexer() {
        lex("
let five = 5;
let ten = 10;
let add = fn(x, y) {
  x + y;
};
let result = add(five, ten);
!-/*5;
5 < 10 > 5;
if (5 < 10) {
	return true;
} else {
    return false;
}
10 == 10;
10 != 9;
\"foobar\"
\"foo bar\"
\"foo \\\"bar\"
[1, 2];
{\"foo\": \"bar\"}
", vec![
            (Let, "let"),
            (Ident, "five"),
            (Assign, "="),
            (Int, "5"),
            (Semicolon, ";"),
            (Let, "let"),
            (Ident, "ten"),
            (Assign, "="),
            (Int, "10"),
            (Semicolon, ";"),
            (Let, "let"),
            (Ident, "add"),
            (Assign, "="),
            (Function, "fn"),
            (LParen, "("),
            (Ident, "x"),
            (Comma, ","),
            (Ident, "y"),
            (RParen, ")"),
            (LBrace, "{"),
            (Ident, "x"),
            (Plus, "+"),
            (Ident, "y"),
            (Semicolon, ";"),
            (RBrace, "}"),
            (Semicolon, ";"),
            (Let, "let"),
            (Ident, "result"),
            (Assign, "="),
            (Ident, "add"),
            (LParen, "("),
            (Ident, "five"),
            (Comma, ","),
            (Ident, "ten"),
            (RParen, ")"),
            (Semicolon, ";"),
            (Bang, "!"),
            (Minus, "-"),
            (Slash, "/"),
            (Asterisk, "*"),
            (Int, "5"),
            (Semicolon, ";"),
            (Int, "5"),
            (LT, "<"),
            (Int, "10"),
            (GT, ">"),
            (Int, "5"),
            (Semicolon, ";"),
            (If, "if"),
            (LParen, "("),
            (Int, "5"),
            (LT, "<"),
            (Int, "10"),
            (RParen, ")"),
            (LBrace, "{"),
            (Return, "return"),
            (True, "true"),
            (Semicolon, ";"),
            (RBrace, "}"),
            (Else, "else"),
            (LBrace, "{"),
            (Return, "return"),
            (False, "false"),
            (Semicolon, ";"),
            (RBrace, "}"),
            (Int, "10"),
            (Eq, "=="),
            (Int, "10"),
            (Semicolon, ";"),
            (Int, "10"),
            (NotEq, "!="),
            (Int, "9"),
            (Semicolon, ";"),
            (Str, "\"foobar\""),
            (Str, "\"foo bar\""),
            (Str, "\"foo \\\"bar\""),
            (LBracket, "["),
            (Int, "1"),
            (Comma, ","),
            (Int, "2"),
            (RBracket, "]"),
            (Semicolon, ";"),
            (LBrace, "{"),
            (Str, "\"foo\""),
            (Colon, ":"),
            (Str, "\"bar\""),
            (RBrace, "}"),
        ]);
    }

    #[test]
    fn nonterminated_string() {
        lex("\n\"fooo\n\n", vec![
            (Illegal, "\"fooo\n\n"),
        ]);
        // assert_eq!(errors[0].col, 0);
        // assert_eq!(errors[0].row, 0);
        // assert_eq!(errors[0].message, String::from(""));
    }

    #[test]
    fn expression_statement() {
        lex("5;", vec![
            (Int, "5"),
            (Semicolon, ";"),
        ]);
    }

    fn lex(input: &str, tokens: Vec<(TokenType, &str)>) {
        let input = String::from(input);
        let lexer = Lexer::new(&input);
        let actual: Vec<Token> = lexer.collect();

        let expected: Vec<Token> = tokens.iter()
            .map(|(type_, literal)| Token {
                type_: *type_,
                literal: String::from(*literal),
                col: 0,
                row: 0,
            })
            .collect();

        assert_eq!(
            token_types(actual.iter()),
            token_types(expected.iter()),
        );

        assert_eq!(
            token_literals(actual.iter()),
            token_literals(expected.iter()),
        );
    }

    fn token_types<'a, I: Iterator<Item = &'a Token>>(it: I) -> Vec<TokenType> {
        it.map(|x| x.type_).collect()
    }

    fn token_literals<'a, I: Iterator<Item = &'a Token>>(it: I) -> Vec<String> {
        it.map(|x| x.literal.clone()).collect()
    }
}
