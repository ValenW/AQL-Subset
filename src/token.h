#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

enum TokenType {KEYWORD, ID, REG, NUM, RSHARP, LSHARP, RPARA, LPARA, RBRA, LBRA, DOT, COLON, END};

static const string TokenTypeString[13] = {"KEYWORD", "ID", "REG", "NUM", "RSHARP", "LSHARP", "RPARA", "LPARA", "RBRA", "LBRA", "DOT", "COLON", "END"};

class Token {
    public:
        string text;
        TokenType type;
        int line;
        int beg;

        Token(const string text, TokenType type, int line, int beg);
        Token(TokenType t);
        Token();
};

#endif
