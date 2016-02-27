/**
 * @Author  : ValenW
 * @Link    : https://github.com/ValenW
 * @Email   : ValenW@qq.com
 * @Date    : 2015-12-12 20:24:12
 * @Last Modified by:   ValenW
 * @Last Modified time: 2015-12-18 22:05:40
 */

#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include "regex.cpp"
#include "output.cpp"
#include "operator.h"

using namespace std;

int main(int argc, char const *argv[]) {
    clock_t start = clock();
    Operator o = Operator(string(argv[1]));
    // cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
    o.operate(string(argv[2]));
    cout << "Time cost: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s" << endl;
}
