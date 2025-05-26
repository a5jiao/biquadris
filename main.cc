#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <set>

#include "Modules/Controller/Controller.h"

using namespace std;



int main(int argc, char *argv[]) {
    int i = 0;
    int t = 0;
    int sd = 0;
    int l = 0;
    string sf1 = "sequence1.txt";
    string sf2 = "sequence2.txt";
    while (i < argc) {
        string s = argv[i];
        if      (s == "-text")  {t = 1;}
        else if (s == "-seed") {
            ++i;
            if (i < argc) {
                s = argv[i];
                istringstream is{s};
                is >> sd;
            }
        }
        else if (s == "-scriptfile1") {
            ++i;
            if (i < argc) {
                s = argv[i];
                istringstream is{s};
                is >> sf1;
            }
        }
        else if (s == "-scriptfile2") {
            ++i;
            if (i < argc) {
                s = argv[i];
                istringstream is{s};
                is >> sf2;
            }
        }
        else if (s == "-startlevel") {
            ++i;
            if (i < argc) {
                s = argv[i];
                istringstream is{s};
                is >> l;
            }
        }
        ++i;
    }
    set<pair<int,int>> s;

    Controller GameController = Controller(t,sd,l,sf1,sf2, s);
    GameController.GameplayLoop();
    return 0;
}
