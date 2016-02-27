#ifndef VIEW_H
#define VIEW_H

#include "view_model.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class View {
    public:
        /* colName 和 talbe 是对应的，即table[0]表示第一列，对应的列名为colName[0]
         * table[i]是一个 size == 2 的vector<int>, 表示表中的一个项，前闭后开
         * 如全文为"There is an apple in the sky.\n..."
         * 则项{6, 8} 为is等等。
         */
        string viewName;
        vector<string> colName;
        vector< vector<int> > table;

        View(ViewModel vm);
        View();
        bool getColon(string cn, vector< pair<int, int> > &v);
        void print();
};

#endif