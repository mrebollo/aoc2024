/*
advent of code day 5 (1)
Identify correct partial topological order
version b) assume page order are true paths
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
using namespace std;

void save(vector<vector<int> > adj, string filename){
    fstream file(filename, ios::out);
    file << "*Vertices " << adj.size() << endl;
    file << "*Edges" << endl;
    for(int i = 0; i < adj.size(); i++){
        for(int j = 0; j < adj[i].size(); j++){
            if(adj[i][j] == 1){
                file << i << " " << j << endl;
            }
        }
    }
    file.close();
}

// process the line to extract the pages
vector<int> process(string line){
    vector<int> pages;
    stringstream ss(line);
    string value;
    while(ss.good()){
        getline(ss, value, ',');
        pages.push_back(stoi(value));
    }
    return pages;
}

// get the center of the sequence
int central(vector<int> &pages){
    return pages[pages.size()/2];
}



// checks if the sequence is a topological order
// i.e. the level of the pages is increasing
bool is_ordered(vector<vector<int> > &adj, vector<int> pages){
    for(int i = 1; i < pages.size(); i++)
        if(adj[pages[i-1]][pages[i]] == 0) return false;
    return true;
}

int main() {
    int u, v; //nodes of the edge
    string line;
    fstream file("input.txt");
    vector<vector<int> > adj(100, vector<int>(100, 0));
    set<int> nodes;
    // extract graph from input
    while(getline(file, line)){
        if(line == "") break;
        // extract the nodes of the edge
        sscanf(line.c_str(), "%d|%d", &u, &v);
        // create the edge
        adj[u][v] = 1;
        // insert the nodes in a set (no repetitions)
        nodes.insert(u);
        nodes.insert(v);
    }
    // extract the pages ordering
    int sum = 0;
    while (getline(file, line)){
        vector<int> pages = process(line);
        // check if the pages are ordered and get the central item
        if(is_ordered(adj, pages)){
            int item = central(pages);
            sum += item;
            cout << "ordered - ";
            cout << "item: " << item << endl;
        }
    }
    
    cout << "sum: " << sum << endl;
    file.close();
    return 0;
}