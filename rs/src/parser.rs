use crate::lexer::Lexer;
use crate::token::TokenType::*;
use crate::ast::*;
use std::iter::Peekable;
use crate::token::{TokenType, Token};

pub fn parse(lex: Lexer) -> Program {
    let p = Parser { lex: lex.peekable() };
    return p.parse();
}

struct Parser<'a> {
    lex: Peekable<Lexer<'a>>,
}

impl<'a> Parser<'a> {
    fn parse(mut self) -> Program {
        let mut p = Program::new();

        while self.lex.peek().is_some() {
            match self.parse_statement() {
                Some(stmt) => p.statements.push(stmt),
                _ => {}
            };
        }

        p
    }

    fn parse_statement(&mut self) -> Option<Statement> {
        self.parse_expression_statement()
    }

    fn parse_expression_statement(&mut self) -> Option<Statement> {
        let exp = match self.parse_expression() {
            Some(expression) => Statement::Expression(expression),
            _ => return None,
        };

        if self.peek_token_is(Semicolon) {
            self.lex.next();
        }

        Some(exp)
    }

    fn parse_expression(&mut self) -> Option<Expression> {
        let token = self.lex.next().unwrap();
        let exp = match token.type_ {
            Int => Expression::new_integer(token),
            True => Expression::new_boolean(token),
            Ident => Expression::new_identifier(token),
            _ => return None,
        };

        Some(exp)
    }

    fn expect_peek(&mut self, t: TokenType) -> bool {
        if self.peek_token_is(t) {
            self.lex.next();
            true
        } else {
            self.peek_error(t);
            false
        }
    }

    fn peek_token_is(&mut self, t: TokenType) -> bool {
        match self.lex.peek() {
            Some(t) => true,
            _ => false,
        }
    }

    fn peek_error(&mut self, t: TokenType) {
        let msg = format!("expected next token to be {:?}, got {:?} instead",
                          t, self.peek_token().unwrap().type_);
        println!("{}", msg);
    }

    fn peek_token(&mut self) -> Option<&Token> {
        self.lex.peek()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parser() {
        let input = String::from("true;");
        let lex = Lexer::new(&input);
        let p = parse(lex);
        dbg!(p);
    }
}
