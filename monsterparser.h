#include <string>
#include <vector>
#include <iostream>

using namespace std;

class monsterparser {
    public:
        string name;
        string symb;
        string desc;
        string color;
        string speed;
        string hp;
        string dam;
        vector<string> abilities;
        monsterparser(void);
        monsterparser(string params[8]);
        void print();
};
