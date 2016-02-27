#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

class Tokenizer {
public:
    /**
     * tokenOrder 存储文章Token的划分（从1开始）。
     * 空白符归于前一个Token，但变为负数，开头的空白符归为-1。
     * 如"  Hello World\n"
     * 则为 {-1-1-1 11111 -1 22222 -2}
     * size表示文章长度，即tokenOrder和buffer的size
     */
    unsigned long size;
    vector<long> tokenOrder;
    string document;
    
    Tokenizer();

    Tokenizer(const char* address);
};

#endif
