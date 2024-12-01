/*
advent of code day 1 (2)
sum similarity score between two lists
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

// find repetitions of n in vector v
int rep(int n, vector<int> &v) {
    int count = 0;
    for (int i : v) 
        if (n == i) 
            count++;    
    return count;
}

// find similarity between two vectors
// count how many times each item of v1 appears in v2
vector<int> similarity(vector<int> &v1, vector<int> &v2) {
    vector<int> sim;
    for(int i: v1) 
        sim.push_back(rep(i, v2));
    return sim;
}

// weighted sum of elements of a vector
int sum(vector<int> &v, vector<int> &sim) {
    int sum = 0;
    for (int i = 0; i < v.size(); i++)
        sum += v[i] * sim[i];
    return sum;
}

int main() {
    vector<int> v1, v2;
    // read file line by line and store in two vectors
    load(v1, v2);
    // obtain similarity
    vector<int> sim = similarity(v1, v2);
    cout << sum(v1, sim) << endl;

    return 0;
}