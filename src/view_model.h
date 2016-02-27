#ifndef VIEW_MODEL_H
#define VIEW_MODEL_H

#include <string>
#include <vector>
#include <string>

using namespace std;

enum CreateWay { Select, Regex, Pattern };
enum RuleType { Tok, Reg, Col };

// 用于存储创建列的规则
class Rule {
    public:
        RuleType type;
        string reg;
        int least, most;
        string colon[2];

        void setColon(vector<string> c) {
            colon[0] = c[0];
            colon[1] = c[1];
        }

        Rule() : least(1), most(1) {}

        Rule(RuleType t, int l = 1, int m = 1) : type(t), least(l), most(m) {}

        Rule(RuleType t, string r, vector<string> vc, int l = 1, int m = 1) : \
            type(t), reg(r), least(l), most(m) { setColon(vc); }

        Rule(RuleType t, vector<string> vc, int l = 1, int m = 1) : \
            type(t), least(l), most(m) { setColon(vc); }
};

// 用于创建View的模板
class ViewModel {
    public:
        CreateWay type;
        string viewName;
        vector<string> colName;
        // 表示第i列内容为第group[i]组的内容
        vector<int> groups;
        // 表示第i组的左边缘为第groupOrders[i*2]条rule,右边缘为第groupOrders[i*2 + 1]条rule
        vector<int> groupOrders;
        vector<Rule> rules;

        ViewModel() {}

        ViewModel(string name) : viewName(name) {}

        void addCol(string cn, int group) {
            colName.push_back(cn);
            groups.push_back(group);
        }

        void addColBySelect(string cn, vector<string> vc) {
            colName.push_back(cn);
            rules.push_back( Rule(Col, vc) );
        }

        void addRule(Rule r) { rules.push_back(r); }
};

#endif
