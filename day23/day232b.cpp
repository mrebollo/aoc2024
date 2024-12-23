/*
    advent of code 23 (2)
    Count computers begining from with 't' that appears in a triade

    largest clique
    known algorithm : Tarjan & Trojanowski (1977) 
    idea: sort by degree and remove nodes with lower degree
    until the graph is a clique
    
    Implementd from a adjacency list using a map
    add a self loop to each node to simplify the algorithm
    take node i
    check if all their neighbors have the same adjacency list
    filter: just nodes with the same degree

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
//#include <boost/graph/undirected_graph.hpp>
//#include <boost/graph/bron_kerbosch_all_cliques.hpp>
//#include "helper.hpp"

using namespace std;

struct Node{
    string name;
    int id;
    int deg; //degree
    //nodes inserted from edges -> degree 1
    Node (string n, int i, int deg = 1): name(n), id(i) {};
};

void println(string comment, vector<string> s){ 
    cout << comment << ": ";
    for(string x : s) 
        cout << x << " ";
    cout << endl;
}


// hash table with chaining for conflict resolution
// not needed: only for names beginning with 't'
class Hash{
    private:
        vector<vector<Node> > table;
        int nnodes;
    public:
        Hash(){table.resize(26); nnodes = 0;}
        int insert(string name){
            int key = name[0] - 'a';
            //check if the value is already in the table
            for(Node v: table[key]){
                if(v.name == name){
                    //try to insert means new edge -> increase degree
                    v.deg++;
                    return v.id;
                }
            }
            //insert if new node
            table[key].push_back(Node(name, nnodes++));
            return nnodes-1;
        }
        inline vector<Node>& get_all(char c){ return table[c - 'a']; }
        inline int size(){ return nnodes; }
};

class Network{
    private:
        Hash node_lst;
        vector<vector<int> > A; //adjacency matrix
        map<string, set<string> > adj_list; //adjacency list directly with labels
        int nnodes;
        vector<string> index;
        int clique_count();
        void adj2list();
    public:
        Network(){nnodes = 0;};
        void load_nodes(string filename);
        void print();
        int clustering(Node u);
        set<string> clique();
        inline vector<Node>& get_all(char c){ return node_lst.get_all(c); }
};


void Network::load_nodes(string filename){
    fstream inputf(filename);
    string line;
    while(getline(inputf, line)){
        stringstream ss(line);
        string u, v;
        //read the edge
        getline(ss, u, '-');
        getline(ss, v);
        //insert the nodes and get their ids
        int uid = node_lst.insert(u);
        int vid = node_lst.insert(v);
        //insert the nodes in the index (for fast access)
        nnodes = node_lst.size();
        index.resize(nnodes+1);
        index[uid] = u;
        index[vid] = v;
        //insert the edge in the adjacency matrix
        A.resize(nnodes);
        for(int i = 0; i < nnodes; i++)
            A[i].resize(nnodes, 0);
        A[uid][vid] = 1;  A[vid][uid] = 1;
    }
    inputf.close();
}


void Network::print(){
        //heading
    cout << "\t";
    for(int i = 0; i < nnodes; i++)
        cout << index[i] << " ";
    cout << endl;
    //values
    for(int i = 0; i < nnodes; i++){
        int deg = 0;
        cout << index[i] << "\t";
        for(int j = 0; j < nnodes; j++){
            cout << A[i][j] << "  ";
            deg += A[i][j];
        }
        cout << deg << endl;
    }
}


// local clustering coefficient Ci = 2*Ei/(ki*(ki-1)) of a node i
// used to count triangles
// to avoid repeat triangles with more than one 't' node
// disconnect the node once counted (row)
int Network::clustering(Node u){
    int id = u.id;
    int Ci = 0;
    //get the neighbors of the node
    for(int i = 0; i < nnodes; i++)
        if(A[id][i] == 1){
            // get "friends of my friends"
            for(int j = i+1; j < nnodes; j++)
                //triangle if all three connected A(x,y) == 1
                Ci += A[id][i] * A[id][j] * A[i][j];
            //remove just the row -> in-degree 0
            A[i][id] = 0;
        }
    return Ci;
}

int count_triangles(Network& nx){
    int triangles = 0;
    //get nodes with name beginning with 't'
    vector<Node> nodes = nx.get_all('t');
    for(Node u : nodes){
        // calculate local clustering coefficient (num triangles)
        int Ci = nx.clustering(u);
        cout << "Node " << u.name << " - " << Ci << " triangles" << endl;
        triangles += Ci;
    }
    return triangles;
}


//determines the number of cliques in the graph
// Number of cliques = n * (n – 1) / 2 – m + 1 
// where n is the number of nodes and m is the number of edges
int Network::clique_count(){
    int edges = 0;
    for(int i = 0; i < nnodes; i++)
        for(int j = i+1; j < nnodes; j++)
            edges += A[i][j];
    return nnodes * (nnodes - 1) / 2 - edges + 1;
}

//convert the adjacency matrix to a list
void Network::adj2list(){
    for(int i = 0; i < nnodes; i++){
        set<string> adj;
        for(int j = 0; j < nnodes; j++)
            if(A[i][j] == 1)
                adj.insert(index[j]);
        adj_list[index[i]] = adj;
    }
}


// obtain the largest clique
set<string> Network::clique(){
    int clqsize = 0;
    set<string> clq;
    adj2list();
    //add loop
    for(int i = 0; i < nnodes; i++){
        adj_list[index[i]].insert(index[i]);
        A[i][i] = 1;
    }
    //compares each adjacency list (if equal, from clique)
    for(const auto &node : adj_list ){
        bool all_same = true;
        set<string> neig = node.second;
        set<string>::iterator itr;
        for (itr = neig.begin(); itr != neig.end(); itr++){
            if(node.second != adj_list[*itr]){
                all_same = false;
                break;
            }
        }
        //if all the same (is a clique) and larger than the current clique, save it
        if(all_same and neig.size() > clqsize){
            clqsize = neig.size();
            clq = neig;
        }
    }

    return clq;
}


int main() {
    Network nx;
    nx.load_nodes("test.txt");
    nx.print();
    //obtain the larges clique
    set<string> clq = nx.clique();
    //short alphabetically for the solution
    //sort(clq.begin(), clq.end());
    cout << "Clique: " << endl;
    for(string node : clq)
        cout << node << ",";
    cout << endl;
    return 0;
}