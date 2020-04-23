#include "monkey/token.h"

using namespace monkey;

CATCH_REGISTER_ENUM(Token::Type,
    Token::Type::ILLEGAL,
    Token::Type::EOF_,
    Token::Type::IDENT,
    Token::Type::INT,
    Token::Type::ASSIGN,
    Token::Type::PLUS,
    Token::Type::COMMA,
    Token::Type::SEMICOLON,
    Token::Type::LPAREN,
    Token::Type::RPAREN,
    Token::Type::LBRACE,
    Token::Type::RBRACE,
    Token::Type::FUNCTION,
    Token::Type::LET)

