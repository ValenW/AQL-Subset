#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <list>
#include <cassert>
#include <dirent.h>
#include "view.h"
#include "parser.h"
#include "tokenizer.h"

extern bool output(View, string, string);
extern vector<vector<int> > findall(const char *regex, const char *content);

typedef struct Tokens {
    int least, most;
    bool cont;
    Tokens(int l = 0, int m = 0, bool c = false) : least(l), most(m), cont(c) {}
} tokens;

class Operator {
private:
    Parser parser;
    Tokenizer tokenizer;
    map<string, View> views;

    // 得到原文中下标index的字符处于第几个token
    int getTokenId(unsigned index);
    // 得到原文中下标index的字符处于的Token的下一个/上一个Token
    pair<int, int> getNext(unsigned index);
    pair<int, int> getLast(unsigned index);

    View creatView(ViewModel vm);

    list< pair<int, int> > getRows(Rule r, list< list<int> > total);

    void append(list< pair<int, int> > &to, list< pair<int, int> > from, bool totalmatch);
public:
    Operator(string path);
    void operate(string path);
};

#endif
