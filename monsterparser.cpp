#include "monsterparser.h"

 monsterparser::monsterparser(void) {
    name = "";
    symb = "";
    desc = "";
    color = "";
    speed = "";
    hp = "";
    dam = "";
}

monsterparser::monsterparser(string params[8]) {
    name = params[0];
    symb = params[1];
    desc = params[2];
    color = params[3];
    speed = params[4];
    //parse abilities
    string s = params[5];
    while(true) {
        int pos = s.find(" ");
        if (pos == string::npos) {
            abilities.push_back(s.substr(0));
            break;
        }
        abilities.push_back(s.substr(0, pos));
        s = s.substr(pos + 1);
    }
    hp = params[6];
    dam = params[7];
}

void monsterparser::print() {
    cout << name << endl;
    cout << desc << endl;
    cout << symb << endl;
    cout << color << endl;
    cout << speed << endl;
    vector<string>::iterator i;
    for (i = abilities.begin(); i != abilities.end(); i++) {
        cout << *i << " ";
    }
    cout << "\n";
    cout << hp << endl;
    cout << dam << endl; 
}

