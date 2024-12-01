/*
advent of code day 1 (1)
sum differences between two ordered lists
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void load(vector<int> &v1, vector<int> &v2) {
    ifstream file("input.txt");
    string line, data;
    while (getline(file, line)) {
        stringstream iss(line);
        // convert string to int using string stream    
        iss >> data;
        v1.push_back(stoi(data));
        iss >> data;
        v2.push_back(stoi(data));
    }
}


int main() {
    vector<int> v1, v2;
    // read file line by line and store in two vectors
    load(v1, v2);
    // sort the vectors
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    // find the sum of differences
    int sum = 0;
    for (int i = 0; i < v1.size(); i++) {
        cout << v1[i] << " " << v2[i] << " " << abs(v1[i]-v2[i]) << endl;
        sum += abs(v2[i] - v1[i]);
    }
    cout << sum << endl;
    return 0;
}