use crate::token::Token;
use crate::token::TokenType::True;

#[derive(Debug)]
pub struct Program {
    pub statements: Vec<Statement>,
}

#[derive(Debug)]
pub enum Statement {
    Block {
        token: Token,
        statements: Vec<Statement>,
    },
    Expression(Expression),
}

#[derive(Debug)]
pub enum Expression {
    Integer {
        token: Token,
        value: isize,
    },
    Boolean {
        token: Token,
        value: bool,
    },
    Identifier {
        token: Token,
        value: String,
    },
}

impl Program {
    pub fn new() -> Program {
        Program { statements: vec![] }
    }
}

impl Expression {
    pub fn new_integer(token: Token) -> Expression {
        Expression::Integer {
            value: token.literal.parse::<isize>().unwrap(),
            token,
        }
    }
    pub fn new_boolean(token: Token) -> Expression {
        Expression::Boolean {
            value: if token.type_ == True { true } else { false },
            token,
        }
    }
    pub fn new_identifier(token: Token) -> Expression {
        Expression::Identifier {
            value: token.literal.clone(),
            token,
        }
    }
}
