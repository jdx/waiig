mod token {
    enum Type {
        ILLEGAL,
        EOF,

        IDENT,
        INT,

        ASSIGN,
        PLUS,

        COMMA,
        SEMICOLON,

        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,

        FUNCTION,
        LET,
    }

    struct Token {
        type_: Type,
        literal: string,
    }
}
