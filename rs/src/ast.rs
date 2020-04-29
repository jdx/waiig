use crate::token::Token;
use crate::token::TokenType::True;

#[derive(Debug)]
pub enum Statement {
    Block(BlockStatement),
    Expression(ExpressionStatement),
}

#[derive(Debug)]
pub enum Expression {
    Integer(IntegerLiteral),
    Boolean(BooleanLiteral),
    Identifier(IdentifierLiteral),
}

#[derive(Debug)]
pub struct Program {
    pub statements: Vec<Statement>,
}

#[derive(Debug)]
pub struct BlockStatement {
    pub token: Token,
    pub statements: Vec<Statement>,
}

#[derive(Debug)]
pub struct ExpressionStatement {
    pub expression: Expression,
}

#[derive(Debug)]
pub struct IntegerLiteral {
    pub token: Token,
    pub value: isize,
}

#[derive(Debug)]
pub struct BooleanLiteral {
    pub token: Token,
    pub value: bool,
}

#[derive(Debug)]
pub struct IdentifierLiteral {
    pub token: Token,
    pub value: String,
}

impl Program {
    pub fn new() -> Program {
        Program { statements: vec![] }
    }
}

impl IntegerLiteral {
    pub fn new(token: Token) -> Expression {
        Expression::Integer(IntegerLiteral {
            value: token.literal.parse::<isize>().unwrap(),
            token,
        })
    }
}

impl BooleanLiteral {
    pub fn new(token: Token) -> Expression {
        Expression::Boolean(BooleanLiteral {
            value: if token.type_ == True { true } else { false },
            token,
        })
    }
}

impl IdentifierLiteral {
    pub fn new(token: Token) -> Expression {
        Expression::Identifier(IdentifierLiteral {
            value: token.literal.clone(),
            token,
        })
    }
}
