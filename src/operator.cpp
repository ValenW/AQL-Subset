#include "operator.h"

int Operator::getTokenId(unsigned index) {
    assert(index >= 0 && index < tokenizer.size);
    if (tokenizer.tokenOrder[index] > 0)
        return tokenizer.tokenOrder[index];
    else
        return -tokenizer.tokenOrder[index];
}

pair<int, int> Operator::getNext(unsigned index) {
    assert(index >= 0 && index < tokenizer.size);
    unsigned begin = index + 1, iorder = getTokenId(index);
    while(begin < tokenizer.size && tokenizer.tokenOrder[begin] <= iorder) begin++;
    if (begin == tokenizer.size) return make_pair(-1, -1);
    unsigned end = begin + 1, border = tokenizer.tokenOrder[begin];
    while(end < tokenizer.size && tokenizer.tokenOrder[end] == border) end++;
    return make_pair(begin, end);
}

pair<int, int> Operator::getLast(unsigned index) {
    assert(index >= 0 && index < tokenizer.size);
    int end = index - 1, iorder = getTokenId(index);
    while( end >= 0 && 
        (tokenizer.tokenOrder[end] >= iorder || tokenizer.tokenOrder[end] < 0) ) 
        end--;
    if (end == -1) return make_pair(-1, -1);
    int begin = end, eorder = tokenizer.tokenOrder[end];
    while(begin >= 0 && tokenizer.tokenOrder[begin] == eorder) begin--;
    return make_pair(begin + 1, end + 1);
}

View Operator::creatView(ViewModel vm) {
    View re(vm);
    switch (vm.type) {
        case Select: {
            for(unsigned i = 0; i < vm.rules.size(); ++i) {
                assert(vm.rules[i].type == Col);
                string vn = vm.rules[i].colon[0], cn = vm.rules[i].colon[1];
                assert(views.find(vn) != views.end());
                vector< pair<int, int> > v;
                if (!(views[vn].getColon(cn, v))) {
                    printf("error: no such colon in view \n");
                    exit(-1);
                }
                if (re.table.size() < v.size()) re.table.resize(v.size());
                for(unsigned j = 0; j < v.size(); ++j) {
                    re.table[j].push_back(v[j].first);
                    re.table[j].push_back(v[j].second);
                }
            }
            break;
        }
        case Regex: {
            assert(vm.rules[0].type == Reg);
            assert(vm.rules[0].colon[0] == "Document");
            assert(vm.rules[0].colon[1] == "text");
            vector< vector<int> > row = findall(vm.rules[0].reg.c_str(), tokenizer.document.c_str());
            re.table.resize(row.size());
            for(unsigned i = 0; i < vm.groups.size(); ++i) {
                int coln = vm.groups[i] * 2;
                for(unsigned j = 0; j < row.size(); ++j) {
                    re.table[j].push_back(row[j][coln]);
                    re.table[j].push_back(row[j][coln + 1]);
                }
            }
            break;
        }
        case Pattern: {
            list< list<int> > total, temptotal;
            tokens token = tokens();
            bool firstNotTokenRule = true;
            for(unsigned i = 0; i < vm.rules.size(); ++i) {
                if (vm.rules[i].type == Tok) {
                    token = tokens(vm.rules[i].least, vm.rules[i].most, true);
                    continue;
                }
                list< pair<int, int> > row = getRows(vm.rules[i], total);
                // 第一次出现不是Token的rule
                if (firstNotTokenRule) {
                    // 首个rule为Token
                    if (token.cont) {
                        for(list< pair<int, int> >::iterator rit = row.begin(); \
                            rit != row.end(); rit++) {
                            int tb = rit->first, te = getLast(tb).second;
                            bool succ = true;
                            for(int i = 0; i < token.least; ++i) {
                                tb = getLast(tb).first;
                                if (tb == -1) { succ = false; break; }
                            }
                            if (!succ) continue;
                            for(int i = token.least; i <= token.most; i++) {
                                list<int> temp = list<int>();
                                temp.push_back(tb);
                                temp.push_back(te);
                                temp.push_back(rit->first);
                                temp.push_back(rit->second);
                                total.push_back(temp);
                                tb = getLast(tb).first;
                                if(tb == -1) break;
                            }
                        }
                        token = tokens();
                    // 首个rule不是Token（即当前rule）
                    } else {
                        for(list< pair<int, int> >::iterator rit = row.begin(); \
                            rit != row.end(); rit++) {
                            list<int> temp = list<int>();
                            temp.push_back(rit->first);
                            temp.push_back(rit->second);
                            total.push_back(temp);
                        }
                    }
                    firstNotTokenRule = false;
                    continue;
                }
                // 将匹配到的row匹配，附加到total上 
                for(list< list<int> >::iterator tit = total.begin(); \
                    tit != total.end(); tit++) {
                    for(list< pair<int, int> >::iterator rit = row.begin(); \
                        rit != row.end(); rit++) {
                        int ttail = getTokenId(tit->back() - 1);
                        int rhead = getTokenId(rit->first);
                        // token.least和token.most默认为0，0
                        if(rhead - ttail > token.least \
                            && rhead - ttail - 1 <= token.most) {
                            temptotal.push_back(*tit);
                            // 之前一条为Token rule则先加入Token的匹配
                            if (token.cont) {
                                int tb = getNext(ttail).first;
                                int te = getLast(rhead).second;
                                assert(tb != -1 && te != -1);
                                temptotal.back().push_back(tb);
                                temptotal.back().push_back(te);
                            }
                            temptotal.back().push_back(rit->first);
                            temptotal.back().push_back(rit->second);
                        }
                    }
                }
                token = tokens();
                total = temptotal;
                temptotal.clear();
                if (total.size() == 0) return re;
            }
            // 最后一条rule为Token
            if (token.cont) {
                temptotal.clear();
                for(list< list<int> >::iterator tit = total.begin(); \
                    tit != total.end(); tit++) {
                    int te = tit->back(), tb = getNext(te).first;
                    bool succ = true;
                    for(int i = 0; i < token.least; ++i) {
                        te = getNext(te).second;
                        if (te == -1) { succ = false; break; }
                    }
                    if (!succ) {
                        list< list<int> >::iterator tempit = tit;
                        tit++;
                        total.erase(tempit);
                        tit--;
                        continue;
                    }
                    tit->push_back(tb);
                    tit->push_back(te);
                    list<int> temp(*tit);
                    for(int i = token.least; i < token.most; i++) {
                        te = getNext(te).second;
                        if (te == -1) break;
                        temp.back() = te;
                        temptotal.push_back(temp);
                    }
                }
                copy(temptotal.begin(), temptotal.end(), back_inserter(total));
                token = tokens();
            }
            vector< vector<int> > table;
            for(list< list<int> >::iterator tit = total.begin(); \
                tit != total.end(); tit++)
                table.push_back(vector<int>(tit->begin(), tit->end()));

            re.table.resize(table.size());
            for(unsigned i = 0; i < vm.groups.size(); ++i) {
                int colN = vm.groups[i] * 2;
                for(unsigned j = 0; j < table.size(); ++j) {
                    re.table[j].push_back(table[j][vm.groupOrders[colN]]);
                    re.table[j].push_back(table[j][vm.groupOrders[colN + 1]]);
                }
            }
            break;
        }
        default: { assert(false); }
    }
    return re;
}

list< pair<int, int> > Operator::getRows(Rule r, list< list<int> > total) {
    list< pair<int, int> > re, row;
    switch(r.type) {
        case Reg: {
            vector< vector<int> > temp = findall(r.reg.c_str(), tokenizer.document.c_str());
            for(unsigned i = 0; i < temp.size(); ++i)
                row.push_back(make_pair(temp[i][0], temp[i][1]));
            break;
        }
        case Col: {
            vector< pair<int, int> > temp;
            assert(views.find(r.colon[0]) != views.end());
            if (!(views[r.colon[0]].getColon(r.colon[1], temp))) {
                printf("error: no such colon in view \n");
                exit(-1);
            }
            row.assign(temp.begin(), temp.end());
            break;
        }
        default: { assert(false); }
    }
    re = row;
    for(int i = 1; i < r.least; ++i) append(re, row, true);
    for(int i = r.least; i < r.most; ++i) append(re, row, false);
    return re;
}

// 将from附加到to后，totalmatch表示to中条目是否必须要匹配至少一个from中的条目
void Operator::append(list< pair<int, int> > &to, list< pair<int, int> > from, \
    bool totalmatch) {
    list< pair<int, int> > temp;
    for(list< pair<int, int> >::iterator toit = to.begin(); toit != to.end(); toit++) {
        bool succ = false;
        for(list< pair<int, int> >::iterator fromit = from.begin(); \
            fromit != from.end(); fromit++) {
            // 如果to恰好是from的前一个token，匹配成功
            if(getTokenId(toit->second - 1) == getTokenId(fromit->first) - 1) {
                if (totalmatch) toit->second = fromit->second;
                else { temp.push_back(make_pair(toit->first, fromit->second)); }
                succ = true;
            }
        }
        if (!succ || totalmatch) {
            list< pair<int, int> >::iterator tempit = toit;
            toit++;
            to.erase(tempit);
            toit--;
        }
    }
    if(!totalmatch) { copy(temp.begin(), temp.end(), back_inserter(to)); }
}

Operator::Operator(string path) { parser = Parser(path.c_str()); }

void Operator::operate(string path) {
    vector<string> files;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(path.c_str())) == NULL) {
        // if the input is a file
        files.push_back(path);
    } else {
        // else a dir
        while( (dirp = readdir(dp)) != NULL ) {
            if (dirp->d_type == 8) files.push_back(path + "/" + dirp->d_name);
        }
    }
    for(unsigned i = 0; i < files.size(); i++) {
        // only files with type ".output" or ".txt" will be operated
        size_t found = files[i].find_last_of(".");
        if (found == string::npos 
            || (files[i].substr(found).compare(".input") != 0
                && files[i].substr(found).compare(".txt") != 0))
            continue;

        // print file name and duel it with tokenizer
        found = files[i].find_last_of("/");
        string fname = found == string::npos ? files[i] : files[i].substr(found + 1);
        printf("Processing %s\n", fname.c_str());
        tokenizer = Tokenizer(files[i].c_str());
        
        // creat views
        for(unsigned i = 0; i < parser.vmList.size(); ++i) {
            string vn = parser.vmList[i].viewName;
            assert(views.find(vn) == views.end());
            views[vn] = creatView(parser.vmList[i]);
        }

        // output views
        for(unsigned i = 0; i < parser.outputList.size(); ++i) {
            assert(views.find(parser.outputList[i].first) != views.end());
            output(views[parser.outputList[i].first], tokenizer.document, parser.outputList[i].second);
        }
        views.clear();
    }
}
