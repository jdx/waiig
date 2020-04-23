package lexer

import (
	"errors"
	"io"

	"monkey/token"
)

type Lexer2 struct {
	input        io.Reader
	buffer       []byte
	position     int  // current position in input (points to current char)
	readPosition int  // current reading position in input (after current ch)
	ch           byte // current char under examination

	eof bool
}

func New2(input io.Reader) *Lexer2 {
	l := &Lexer2{input: input}
	l.readChar()
	return l
}

func (l *Lexer2) NextToken() token.Token {
	var tok token.Token

	l.skipWhitespace()

	switch l.ch {
	case '=':
		tok = newToken(token.ASSIGN, l.ch)
	case ';':
		tok = newToken(token.SEMICOLON, l.ch)
	case '(':
		tok = newToken(token.LPAREN, l.ch)
	case ')':
		tok = newToken(token.RPAREN, l.ch)
	case ',':
		tok = newToken(token.COMMA, l.ch)
	case '+':
		tok = newToken(token.PLUS, l.ch)
	case '{':
		tok = newToken(token.LBRACE, l.ch)
	case '}':
		tok = newToken(token.RBRACE, l.ch)
	case 0:
		tok.Literal = ""
		tok.Type = token.EOF
	default:
		if isLetter(l.ch) {
			tok.Literal = l.readIdentifier()
			tok.Type = token.LookupIdent(tok.Literal)
			return tok
		} else if isDigit(l.ch) {
			tok.Type = token.INT
			tok.Literal = l.readNumber()
			return tok
		} else {
			tok = newToken(token.ILLEGAL, l.ch)
		}
	}
	l.readChar()
	return tok
}

func (l *Lexer2) readChar() {
	// if l.eof {
	// 	return
	// }
	if len(l.buffer) == 0 {
		l.buffer = make([]byte, 8)
		_, err := l.input.Read(l.buffer)
		if err != nil {
			mustEOF(err)
			l.eof = true
			l.ch = 0
			return
		}
	}

	l.ch = l.buffer[0]
	l.buffer = l.buffer[1:]
}

func (l *Lexer2) skipWhitespace() {
	for isWhitespace(l.ch) {
		l.readChar()
	}
}

func (l *Lexer2) readIdentifier() string {
	var ident []byte
	for isLetter(l.ch) {
		ident = append(ident, l.ch)
		l.readChar()
	}
	return string(ident)
}

func (l *Lexer2) readNumber() string {
	var body []byte
	for isDigit(l.ch) {
		body = append(body, l.ch)
		l.readChar()
	}
	return string(body)
}

func mustEOF(err error) {
	if !errors.Is(err, io.EOF) {
		panic(err)
	}
}
