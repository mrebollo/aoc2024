/*
advent of code day 2 (2)
monotonic ordered vector 
including one bad level of tolerance 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


vector<int> str2vector(string line){
    vector<int> v;
    stringstream iss(line);
    string data;
    while(iss >> data)
        v.push_back(stoi(data));
    return v;
}


int bad_level(vector<int> &v){
    int sign = v[1] - v[0];
    if (sign == 0 || abs(v[1] - v[0]) > 3)
        return 0;
    for(int i = 1; i < v.size()-1; i++)
        if(sign * (v[i+1] - v[i]) <= 0 || abs(v[i+1] - v[i]) > 3)
            return i;
    return -1;
}

bool is_secure(vector<int> &v){
    int bad = bad_level(v);
    if(bad == -1)
        return true;
    else{
        vector<int> v1 = v;
        vector<int> v2 = v;
        vector<int> v3 = v;
        v1.erase(v1.begin() + bad-1);
        v2.erase(v2.begin() + bad);
        v3.erase(v3.begin() + bad+1);
        bool sec1 = (bad_level(v1) == -1);
        bool sec2 = (bad_level(v2) == -1);
        bool sec3 = (bad_level(v3) == -1);
        return sec1 || sec2 || sec3;
    }
}


int count_safe(){
    ifstream file("input.txt");
    string line;
    vector<int> v;
    int count = 0;
    while (getline(file, line)) {
        v = str2vector(line);
        bool sec = is_secure(v);
        if(sec){
            count++;
        }
        cout << line << " - sec: " << sec << "(" << count << ")" << endl;
    }
    return count;
}

int main(){
    cout << count_safe() << endl;
    return 0;
}


