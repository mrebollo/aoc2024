/*
advent of code day 19 (1)
identify vallid patterns formed from strips
-> valid words of a formal grammar
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
using namespace std;

string strips;



// if strips are a, b, c, the regular expression is
// [[a*b*c*][a*b*c*][a*b*c*]]+
void generate_expression(string s){
    int len = 0;
    strips += "[";
    stringstream ss(s);
    string token = "[";
    while( ss.good() ){
        string combination;
        getline( ss, combination, ',' );
        token += combination + "*";
        len++;
    }
    token += "]";
    //concatenate tokens n times
    for (int i = 0; i < len; i++)
        strips += token;
    strips += "]+";
}

bool is_valid(string s){
    regex r(strips);
    smatch m;
    return regex_match(s, m, r);
}


int main() {
    ifstream file("test.txt");
    string line;
    //read strips from file
    getline(file, line);
    generate_expression(line);
    cout << "strips: " << strips << endl;
    //blank line
    getline(file, line);
    //read and analayze combinations
    int total_valid = 0;
    while(getline(file, line)){
        if (is_valid(line)){
            total_valid++;
            cout << line << " is valid [" << total_valid << "]" << endl;
        }
        else
            cout << line << " is invalid" << endl;
    }
    cout << "total: " << total_valid << endl;

    return 0;
}