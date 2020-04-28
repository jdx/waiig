use phf::{phf_map};

#[derive(Debug, PartialEq, Copy, Clone)]
pub enum TokenType {
    Illegal,
    // EOF,

    Ident,
    Int,
    Str,

    Assign,
    Plus,
    Minus,
    Bang,
    Asterisk,
    Slash,

    LT,
    GT,
    Eq,
    NotEq,

    Comma,
    Semicolon,
    Colon,

    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,

    Function,
    Let,
    True,
    False,
    If,
    Else,
    Return,
}

#[derive(Debug, PartialEq)]
pub struct Token {
    pub type_: TokenType,
    pub literal: String,
}

static KEYWORDS: phf::Map<&'static str, TokenType> = phf_map! {
    "fn" => TokenType::Function,
    "let" => TokenType::Let,
    "return" => TokenType::Return,
    "if" => TokenType::If,
    "else" => TokenType::Else,
    "true" => TokenType::True,
    "false" => TokenType::False,
};

pub fn lookup_ident(ident: &String) -> TokenType {
    match KEYWORDS.get(ident as &str) {
        Some(t) => *t,
        _ => TokenType::Ident,
    }
}
