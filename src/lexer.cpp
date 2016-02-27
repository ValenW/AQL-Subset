#include "lexer.h"

bool Lexer::isNum(const string& token) {
    for (unsigned i = 0; i < token.size(); i++) {
        if (token[i] > '9' || token[i] < '0') return false;
    }
    return true;
}

void Lexer::addToken(vector<Token>& tokenList, string& token, int line, int beg) {
    if (token == "") return;
    if (token == "create" || token == "view" || token == "as" || token == "output" || token == "select" ||
        token == "from" || token == "extract" || token == "regex" || token == "on" || token == "return" ||
        token == "group" || token == "and" || token == "Token" || token == "pattern") {
        tokenList.push_back(Token(token, KEYWORD, line, beg));
    } else if (isNum(token)){
        tokenList.push_back(Token(token, NUM, line, beg));
    } else {
        tokenList.push_back(Token(token, ID, line, beg));
    }
    token = "";
}

Lexer::Lexer(const char* addr) {
    ifstream document(addr);
    int line = 1;
    int beg = -1;
    char tmp;
    string token = "";
    while (document.get(tmp)) {
        beg++;
        switch (tmp) {
            case '/':{
                addToken(tokenList, token, line, beg);
                while (document.get(tmp)) {
                    if (tmp == '/') {
                        tokenList.push_back(Token(token, REG, line, beg));
                        token = "";
                        break;
                    } else {
                        token += tmp;
                    }
                }
                break;
            }
            case ' ':case '\t':case '\r':case';': {addToken(tokenList, token, line, beg); break;}
            case '\n':{addToken(tokenList, token, line, beg); line++; beg = -1; break;}
            case '.': {addToken(tokenList, token, line, beg); tokenList.push_back(Token(".", DOT, line, beg));break;}
            case ',': {addToken(tokenList, token, line, beg); tokenList.push_back(Token(",", COLON, line, beg));break;}
            case '<': {addToken(tokenList, token, line, beg); tokenList.push_back(Token("<", LSHARP, line, beg));break;}
            case '>': {addToken(tokenList, token, line, beg); tokenList.push_back(Token(">", RSHARP, line, beg));break;}
            case '{': {addToken(tokenList, token, line, beg); tokenList.push_back(Token("{", LBRA, line, beg));break;}
            case '}': {addToken(tokenList, token, line, beg); tokenList.push_back(Token("}", RBRA, line, beg));break;}
            case '(': {addToken(tokenList, token, line, beg); tokenList.push_back(Token("(", LPARA, line, beg));break;}
            case ')': {addToken(tokenList, token, line, beg); tokenList.push_back(Token(")", RPARA, line, beg));break;}
            default: {token += tmp;}
        }
    }
    if (token != "") addToken(tokenList, token, line, beg);
    document.close();
    tokenList.push_back(Token(END));
    idNow = 0;
}

Lexer::Lexer() {}

Token Lexer::getNext() {
    assert(idNow < tokenList.size());
    return tokenList[idNow++];
}

Token Lexer::look() {
    assert(idNow < tokenList.size());
    return tokenList[idNow];
}

bool Lexer::match(TokenType t) {
    if (idNow < tokenList.size() && tokenList[idNow].type == t) {
        idNow++;
        return true;
    }
    return false;
}

bool Lexer::match(string s) {
    if (idNow < tokenList.size() && tokenList[idNow].text == s) {
        idNow++;
        return true;
    }
    return false;
}

Token Lexer::get() {
    assert(idNow - 1 >= 0 && idNow - 1 < tokenList.size());
    return tokenList[idNow - 1];
}
