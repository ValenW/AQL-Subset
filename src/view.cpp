#include "view.h"

View::View() {}

View::View(ViewModel vm) {
    viewName = vm.viewName;
    colName = vm.colName;
}

bool View::getColon(string cn, vector< pair<int, int> > &v) {
    unsigned coln = 0;
    while(coln < colName.size() && colName[coln] != cn) coln++;
    if (coln >= colName.size()) return false;
    for(unsigned i = 0; i < table.size(); ++i)
        v.push_back(make_pair(table[i][coln * 2], table[i][coln * 2 + 1]));
    return true;
}

void View::print() {
    cout << "Viewname: " << viewName << endl
      << "Colons:" << endl;
    for(unsigned i = 0; i < colName.size(); ++i)
        cout << colName[i] << "  ";
    cout << endl;
    for(unsigned i = 0; i < table.size(); ++i) {
        for(unsigned j = 0; j + 1 < table[i].size(); j += 2) {
            cout << table[i][j] << "-" << table[i][j+1] << ", ";
        }
        cout << endl;
    }
}
