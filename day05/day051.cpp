/*
advent of code day 5 (1)
Identify correct partial topological order
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
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

int zero_indegree(vector<vector<int> > &adj){
    for(int i = 0; i < adj.size(); i++){
        int sum = 0;
        for(int j = 0; j < adj.size(); j++)
            sum += adj[j][i];
        if(sum == 0) return i;
    }
    return -1;
}   


// get the levels of the graph
vector<int> levels(vector<vector<int> > &adj){
    vector<int> level(adj.size(), -1);
    queue<int> q;
    int source = zero_indegree(adj);
    q.push(source);
    level[source] = 0;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for(int v = 0; v < adj.size(); v++){
            if(adj[u][v] == 1 & level[v] == -1){
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return level;
}


// checks if the sequence is a topological order
// pages is ordered if it's a valid path in the graph
// from pages[0] to pages[pages.size()-1]
int is_ordered(vector<int> pages, vector<vector<int> > &adj){

    return 1;
}

int main() {
    int u, v; //nodes of the edge
    string line;
    fstream file("input.txt");
    vector<vector<int> > adj(100, vector<int>(100, 0));
    // extract graph from input
    while(getline(file, line)){
        if(line == "") break;
        sscanf(line.c_str(), "%d|%d", &u, &v);
        adj[u][v] = 1;
    }
    save(adj, "input.net");
    vector<int> level = levels(adj);
    // extract the pages ordering
    int sum = 0;
    while (getline(file, line)){
        vector<int> pages = process(line);
        // check if the pages are ordered and get the central item
        if(is_ordered(pages, adj)){
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