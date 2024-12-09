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

// a nodes has zero indegree if sum of the column is zero
int zero_indegree(vector<vector<int> > &adj, set<int> &nodes){
    // looks for nodes in columns
    for(int j: nodes){
        int sum = 0;
        // cover all rows for the selected node j
        for(int i : nodes)
            sum += adj[i][j];
        if(sum == 0) return j;
    }
    return -1;
}   

int indegree(vector<vector<int> > &adj, int node){
    int sum = 0;
    for(int i = 0; i < adj.size(); i++)
        sum += adj[i][node];
    return sum;
}


// get the levels of the graph
vector<int> levels(vector<vector<int> > adj, set<int> nodes){
    vector<int> level(adj.size(), -1);
    queue<int> q;
    int source = zero_indegree(adj, nodes);
    q.push(source);
    level[source] = 0;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        //remove ingoing edges from node u (column)
        for(int i: nodes)
            adj[i][u] = 0;  
        for(int v: nodes){
            if(adj[u][v] == 1){
                //remove de outgoing edge (disconnect)
                adj[u][v] = 0;
                if(indegree(adj, v) == 0){
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
    }
    return level;
}


// checks if the sequence is a topological order
// i.e. the level of the pages is increasing
bool is_ordered(vector<int> pages, vector<int> level){
    for(int i = 0; i < pages.size()-1; i++)
        if(level[pages[i]] > level[pages[i+1]]) return false;
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
    save(adj, "input.net");
    vector<int> level = levels(adj, nodes);
    // extract the pages ordering
    int sum = 0;
    while (getline(file, line)){
        vector<int> pages = process(line);
        // check if the pages are ordered and get the central item
        if(is_ordered(pages, level)){
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