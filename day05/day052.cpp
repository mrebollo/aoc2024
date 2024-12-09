/*
advent of code day 5 (2)
Identify correct partial topological order
Correct wrong page order as use them as solution
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
using namespace std;

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

// reorder the pages to make them ordered
void reorder(vector<vector<int> > &adj, vector<int> &pages){
    for(int i = 0; i < pages.size()-1; i++){
        for(int j = i+1; j < pages.size(); j++){
            if(adj[pages[i]][pages[j]] == 0){
                swap(pages[i], pages[j]);
            }
        }
    }
}

// print the pages
void print(vector<int> pages){
    for(int i = 0; i < pages.size(); i++){
        cout << pages[i] << " ";
    }
    cout << endl;
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
        if(!is_ordered(adj, pages)){
            reorder(adj, pages);
            print(pages);
            int item = central(pages);
            sum += item;
            cout << "reordered - ";
            cout << "item: " << item << endl;
        }
    }
    
    cout << "sum: " << sum << endl;
    file.close();
    return 0;
}