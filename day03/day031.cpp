/*
advent of code day 3 (1)
extract substrings follwing a pattern
usage of regular expressions
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


int process(string s){
    int sumprod = 0;
    //detects the pattern mul(a,b) where a and b are integers
    regex r("mul\\([0-9]+,[0-9]+\\)");
    smatch m; 
    while(regex_search(s, m, r)){
        for (auto x : m){
            sumprod += prod(x);
            //cout << x << " = " << prod(x) << endl;
        }
        //remove the matched part
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