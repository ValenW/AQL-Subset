#include "parser.h"

#define lmatch(a) lexer.match(a)
#define look() lexer.look()
#define get() lexer.get()

using namespace std;

Parser::Parser(string path) {
    lexer = Lexer(path.c_str());
    views.insert("Document");
    AQLs();
}
Parser::Parser() {}

void Parser::paser_error(string s) {
    printf("error found in line %d, colon %d :\n\t\t%s\n", \
        lexer.getNext().line, lexer.getNext().beg, s.c_str());
    exit(-1);
}
void Parser::enmatch(TokenType t) {
    if (!lmatch(t)) {
        string e = "expect a '" + TokenTypeString[t] + "' but '" +
            TokenTypeString[lexer.getNext().type] + "' found.";
        paser_error(e);
    }
}
void Parser::enmatch(string s) {
    if (!lmatch(s)) {
        string e = "expect a '" + s + "' but '" + 
            lexer.getNext().text + "' founded.";
        paser_error(e);
    }
}

void Parser::AQLs() { do { aql_stmt(); } while (look().type != END); }

void Parser::aql_stmt() {
    if (look().text == "create") create_stmt();
    else if (look().text == "output") output_stmt();
    else {
        printf("%s\n", look().text.c_str());
        paser_error("a aql should be leading by 'creat' or 'output'.");
    }
}

void Parser::output_stmt() {
    enmatch("output");
    enmatch("view");
    enmatch(ID);
    string viewName = get().text;
    if (views.find(viewName) == views.end()) {
        string s = "no such view named " + viewName + " .\n";
        paser_error(s);
    }
    string alia = "";
    alias(alia);
    outputList.push_back(make_pair(viewName, alia));
}

bool Parser::alias(string &alia) {
    if (lmatch("as") && lmatch(ID)) { alia =  get().text; return true; }
    return false;
}

void Parser::create_stmt() {
    enmatch("create");
    enmatch("view");
    enmatch(ID);
    string viewName = get().text;
    if (views.find(viewName) != views.end()) {
        string s = "a view named " + viewName + " has already exist.\n";
        paser_error(s);
    }
    ViewModel vm(get().text);
    enmatch("as");
    view_stmt(vm);
    vmList.push_back(vm);
    views.insert(viewName);
}

void Parser::view_stmt(ViewModel &vm) {
    if (look().text == "select") select_stmt(vm);
    else if (look().text == "extract") extract_stmt(vm);
    else paser_error("a view stmt should be leading by 'select' or 'extract'.");
}

void Parser::select_stmt(ViewModel &vm) {
    enmatch("select");
    vm.type = Select;
    vector< vector<string> > selectList = select_list();
    enmatch("from");
    vector< vector<string> > fromList = from_list();

    for(unsigned i = 0; i < fromList.size(); ++i) {
        for(unsigned j = 0; j < selectList.size(); ++j) {
            if(selectList[j][0] == fromList[i][1]) {
                // TODO may bug when name conflict with alia 
                selectList[j][0] = fromList[i][0];
            }
        }
    }
    for(unsigned i = 0; i < selectList.size(); ++i) {
        if(selectList[i].size() == 2) selectList[i].push_back(selectList[i][1]);
        vm.addColBySelect(selectList[i][2], selectList[i]);
    }
}

vector< vector<string> > Parser::select_list() {
    vector< vector<string> > selectList;
    do {
        select_item(selectList);
    } while (lmatch("DOT"));
    return selectList;
}

void Parser::select_item(vector< vector<string> > &l) {
    enmatch(ID);
    vector<string> re;
    re.push_back(get().text);
    enmatch(DOT);
    enmatch(ID);
    re.push_back(get().text);
    string alia;
    if(alias(alia)) re.push_back(alia);
    l.push_back(re);
}

vector< vector<string> > Parser::from_list() {
    vector< vector<string> > fromList;
    do {
        from_item(fromList);
    } while (lmatch(COLON));
    return fromList;
}

void Parser::from_item(vector< vector<string> > &l) {
    enmatch(ID);
    string viewName = get().text;
    if (views.find(viewName) == views.end()) {
        string s = "no such view named " + viewName + ".\n";
        paser_error(s);
    }
    vector<string> re;
    re.push_back(viewName);
    enmatch(ID);
    re.push_back(get().text);
    l.push_back(re);
}

void Parser::extract_stmt(ViewModel& vm) {
    enmatch("extract");
    if (look().text == "regex") regex_stmt(vm);
    else if (look().text == "pattern") pattern_stmt(vm);
    else paser_error("a extract stmt should be leading by 'regex' or 'pattern'.");
    enmatch("from");
    vector< vector<string> > fromList = from_list();
    // TODO to make take formlist a shorter function.
    for(unsigned i = 0; i < fromList.size(); ++i) {
        for(unsigned j = 0; j < vm.rules.size(); ++j) {
            if(vm.rules[j].colon[0] == fromList[i][1]) {
                vm.rules[j].colon[0] = fromList[i][0];
            }
        }
    }
}

void Parser::regex_stmt(ViewModel& vm) {
    enmatch("regex");
    vm.type = Regex;
    enmatch(REG);
    string reg = get().text;
    enmatch("on");
    vector<string> col = colon_stmt();
    vm.addRule(Rule(Reg, reg, col));
    vector< pair<string, int> > colName = col_name_stmt();
    for(unsigned i = 0; i < colName.size(); ++i)
        vm.addCol(colName[i].first, colName[i].second);
}

vector<string> Parser::colon_stmt() {
    enmatch(ID);
    vector<string> re;
    re.push_back(get().text);
    enmatch(DOT);
    enmatch(ID);
    re.push_back(get().text);
    return re;
}

vector< pair<string, int> > Parser::col_name_stmt() {
    if (lmatch("as")) {
        vector< pair<string, int> > re;
        enmatch(ID);
        re.push_back(make_pair(get().text, 0));
        return re;
    }
    enmatch("return");
    return group_stmt();
}

vector< pair<string, int> > Parser::group_stmt() {
    vector< pair<string, int> > re;
    do {
        group_item(re);
    } while (lmatch("and"));
    return re;
}

void Parser::group_item(vector< pair<string, int> > &l) {
    enmatch("group");
    enmatch(NUM);
    int groupNum = atoi(get().text.c_str());
    enmatch("as");
    enmatch(ID);
    l.push_back(make_pair(get().text, groupNum));
}

void Parser::pattern_stmt(ViewModel& vm) {
    enmatch("pattern");
    vm.type = Pattern;
    Rule r;
    int groupNum = 0, ruleNum = 0;
    stack<int> q;
    bool c = true;
    while (c) {
        while(lmatch(LPARA)) {
            q.push(++groupNum);
            if (vm.groupOrders.size() < (unsigned)(groupNum + 1) * 2)
                vm.groupOrders.resize((groupNum + 1) * 2);
            vm.groupOrders[groupNum * 2] = ruleNum * 2;
        }

        if (!pattern_item(r)) break;
        vm.addRule(r); ruleNum++;

        while(lmatch(RPARA)) {
            if (q.empty()) paser_error("Brackets do not match!");
            vm.groupOrders[q.top() * 2 + 1] = ruleNum * 2 - 1;
            q.pop();
        }
    }
    if (!q.empty()) paser_error("Brackets do not match!");
    if (vm.groupOrders.size() < 2) vm.groupOrders.resize(2);
    vm.groupOrders[0] = 0;
    vm.groupOrders[1] = ruleNum * 2 - 1;

    vector< pair<string, int> > colName = col_name_stmt();
    for(unsigned i = 0; i < colName.size(); ++i)
        vm.addCol(colName[i].first, colName[i].second);
}

bool Parser::pattern_item(Rule &r) {
    r = Rule();
    if (!pattern_atom(r)) return false; // ensure no group like ()
    if (lmatch(LBRA)) {
        enmatch(NUM);
        r.least = atoi(get().text.c_str());
        enmatch(COLON);
        enmatch(NUM);
        r.most = atoi(get().text.c_str());
        enmatch(RBRA);
    }
    return true;
}

bool Parser::pattern_atom(Rule &r) {
    if (lmatch(REG)) {
        r.type = Reg;
        r.reg = get().text;
        return true;
    } else if (lmatch(LSHARP)) {
        if(lmatch("Token")) r.type = Tok;
        else { r.type = Col; r.setColon(colon_stmt()); }
        enmatch(RSHARP);
        return true;
    }
    return false;
}
