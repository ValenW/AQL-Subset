#include "token.h"

Token::Token(const string text, TokenType type, int line, int beg) {
 this->text = text;
 this->type = type;
 this->line = line;
 this->beg = beg;
}

Token::Token(TokenType t) : type(t) {}

Token::Token() {}
