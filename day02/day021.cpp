/*
advent of code day 2 (1)
identify ascending/descending vectors
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// secure if
// 1. vector ascending or descending (monotonic)
// 2. difference at most 3
bool is_secure(vector<int> &v){
    // get the direction with the two first elements
    // > 0 ascend, < 0 descend
    int sign = v[1] - v[0];
    for (int i = 0; i < v.size()-1; i++){
        // direction changes if changes sign if differences
        // max diff of 3 (in abs() to avoid signs)
        if (sign * (v[i+1] - v[i]) <= 0 || abs(v[i+1] - v[i]) > 3)
            return false;
    }
    return true;
}

// converts a string to a vector of integers
vector<int> str2vector(string &line){
    vector<int> v;
    stringstream iss(line);
    string data;
    while (iss >> data){
        v.push_back(stoi(data));
    }
    return v;
}

// counts the number of secure vectors from file
int count_secure(){
    ifstream file("input.txt");
    string line;
    vector<int> v;
    int count = 0;
    while (getline(file, line)) {
        vector<int> v = str2vector(line);
        if (is_secure(v)){
            count++;
            cout << line << " - secure: " << count << endl;
        }
    }
    return count;
}


int main(){
    cout << count_secure() << endl;
    return 0;
}