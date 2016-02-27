#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include "token.h"

using namespace std;

class Lexer {
    private:
        vector<Token> tokenList;
        unsigned long idNow;

        bool isNum(const string& token);

        void addToken(vector<Token>& tokenList, string& token, int line, int beg);
    public:
        Lexer(const char* addr);

        Lexer();
        
        Token getNext();

        Token look();

        bool match(TokenType t);

        bool match(string s);

        Token get();
};

#endif