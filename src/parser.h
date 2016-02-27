#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <cstdlib>
#include <cassert>
#include <utility>
#include "lexer.h"
#include "view_model.h"

using namespace std;

class Parser {
private:
    Lexer lexer;
    // 用于存储当前已经创建的View的viewName，用于判断引用不存在的View等语法错误
    set<string> views;
    void paser_error(string s);
    // enmatch表示下一个Token必须成功match，否则报错
    void enmatch(TokenType t);
    void enmatch(string s);

    // 以下将语法分析的每个语法子句都写成了一个函数，语法树跟PPT上的基本一致
    // AQLs -> aql_stmt | ^
    void AQLs();

    // aql_stmt -> output_stmt | create_stmt
    void aql_stmt();

    // output_stmt -> "output view" ID alias
    void output_stmt();

    // alias -> "as" ID | ^
    bool alias(string &alia);

    // create_stmt -> "create view" ID "as" view_stmt
    void create_stmt();

    // view_stmt -> select_stmt | extract_stmt
    void view_stmt(ViewModel &vm);

    // select_stmt -> "select" select_list "from" from_list
    void select_stmt(ViewModel &vm);

    // select_list -> select_item | select_item "," select_list
    vector< vector<string> > select_list();

    // select_item -> ID "." ID alias
    void select_item(vector< vector<string> > &l);

    // from_list -> from_item | from_item "," from_list
    vector< vector<string> > from_list();

    // from_item -> ID ID
    void from_item(vector< vector<string> > &l);

    // extract_stmt -> "extract" (regex_stmt | pattern_stmt) "from" from_list
    void extract_stmt(ViewModel& vm);

    // regex_spec -> "regex" REG "on" colon_stmt col_name_stmt
    void regex_stmt(ViewModel& vm);

    // colon_stmt -> ID "." ID
    vector<string> colon_stmt();

    // col_name_stmt -> "as" ID | "return" group_stmt
    vector< pair<string, int> > col_name_stmt();

    // group_stmt -> group_item | group_item "and" group_stmt
    vector< pair<string, int> > group_stmt();

    // group_item -> "group" NUM "as" ID
    void group_item(vector< pair<string, int> > &l);

    // pattern_stmt -> "pattern" pattern_item+ col_name_stmt
    void pattern_stmt(ViewModel& vm);

    // pattern_item -> "("+ pattern_atom ("{" NUM "," NUM "}")? ")"+
    bool pattern_item(Rule &r);

    // pattern_atom -> REG | ("<" ("Token" | colon_stmt) ">")
    bool pattern_atom(Rule &r);

public:
    vector<ViewModel> vmList;
    vector< pair<string, string> > outputList;
    Parser(string path);
    Parser();
};

#endif