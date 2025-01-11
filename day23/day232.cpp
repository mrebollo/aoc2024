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
    - (?) check if all their neighbors have the same adjacency list (not neccesary)
    - check the longest list of common nodes (intersection)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>


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
        cout << x << ",";
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
        Node& get_node(string name){
            int i = name[0] - 'a';
            for(Node& v: table[i])
                if(v.name == name)
                    return v;
        }
        inline int size(){ return nnodes; }
};

class Network{
    private:
        Hash node_lst;
        vector<vector<int> > A; //adjacency matrix
        unordered_map<int, unordered_set<int> > adj_list; //adjacency list (map)
        int nnodes;
        vector<string> index;
        void adj2list();
        vector<string>largest(vector<unordered_set<int> >& cliques);
    public:
        Network(){nnodes = 0;};
        void load_nodes(string filename);
        void print();
        vector<string> clique();
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



//convert the adjacency matrix to adjacency lists
void Network::adj2list(){
    for(int i = 0; i < nnodes; i++){
        unordered_set<int> adj;
        for(int j = 0; j < nnodes; j++)
            if(A[i][j] == 1)
                adj.insert(j);
        adj_list[i] = adj;
    }
}


void bronKerbosch(
    unordered_set<int>&& R, unordered_set<int>&& P,
    unordered_set<int>&& X,
    unordered_map<int, unordered_set<int> >& graph,
    vector<unordered_set<int> >& cliques)
{
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
        return;
    }

    while (!P.empty()) {
        int v = *P.begin();
        unordered_set<int> newR = R;
        newR.insert(v);
        unordered_set<int> newP;
        for (int p : P) {
            if (graph[v].find(p) != graph[v].end()) {
                newP.insert(p);
            }
        }
        unordered_set<int> newX;
        for (int x : X) {
            if (graph[v].find(x) != graph[v].end()) {
                newX.insert(x);
            }
        }
        bronKerbosch(move(newR), move(newP), move(newX),
                     graph, cliques);
        P.erase(v);
        X.insert(v);
    }
}

//gets the largest clique from the list and returns in a vector with the labels
vector<string> Network::largest(vector<unordered_set<int> >& cliques){
    int clqsize = 0;
    unordered_set<int>largest;
    for(unordered_set<int>& clq : cliques){
        if(clq.size() > clqsize){
            clqsize = clq.size();
            largest = clq;
        }
    }
    vector<string> result;
    for(int i : largest)
        result.push_back(index[i]);
    return result;
}


// obtain the largest clique
vector<string> Network::clique(){
    adj2list();
    vector<unordered_set<int> > allCliques;
    //initialize the sets (literalls {} not allowed)
    unordered_set<int> P, X, R;
    for(int i = 0; i < nnodes; i++)
        P.insert(i);
    bronKerbosch(move(X), move(P), move(R), adj_list, allCliques);
    return largest(allCliques);
}


int main() {
    Network nx;
    nx.load_nodes("input.txt");
    //nx.print();
    //obtain the larges clique
    vector<string> clq = nx.clique();
    //short alphabetically for the solution
    sort(clq.begin(), clq.end());
    println("Clique", clq);
    return 0;
}