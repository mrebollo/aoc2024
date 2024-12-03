/*
advent of code day 3 (2)
extract substrings follwing a pattern
usage of regular expressions with activation/deactivation (do()-don't())
*/

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

int prod(string s){
    int a, b;
    sscanf(s.c_str(), "mul(%d,%d)", &a, &b);
    return a * b;
}

int process(string line){
    int sumprod = 0;
    string s = line;
    // detecting three patterns mul() do() don't()
    regex r("mul\\([0-9]+,[0-9]+\\)|do\\(\\)|don't\\(\\)");
    smatch m; 
    // actually it is a unique line
    // make it static to keep state between calls
    // just in case a line ends deactivated
    static bool active = true;
    while (regex_search(s, m, r)){
        //reactivate with do()
        if(m[0] == "do()"){
            active = true;
            cout << m[0] << " activated" << endl;
        }
        //deactivate with don't()
        else if(m[0] == "don't()"){
            active = false;
            cout << m[0] << " deactivated" << endl;
        }
        // is a mult pattern, process if active
        else if(active){
            sumprod += prod(m[0]);
            cout << m[0] << " = " << prod(m[0]) << " processed " << endl;
        }
        else{
            cout << m[0] << " skipped" << endl;
        }
        s = m.suffix().str();
    }
    return sumprod;
}

int main() {
    ifstream file("input.txt");
    string line;
    int sum = 0, prod, id = 0;
    while(getline(file, line)){
        prod = process(line);
        cout << ++id << ": " << prod << endl;
        sum += prod;
    }
    cout << "total: " << sum << endl;
    return 0;
}