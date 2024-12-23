/*
    advent of code 23 (2)
    Count computers begining from with 't' that appears in a triade

    largest clique
    known algorithm : Tarjan & Trojanowski (1977) 
    idea: sort by degree and remove nodes with lower degree
    until the graph is a clique
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
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
        int nnodes;
        vector<string> index;
    public:
        Network(){nnodes = 0;};
        void load_nodes(string filename);
        void print();
        int clustering(Node u);
        vector<string> clique();
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
        cout << index[i] << "\t";
        for(int j = 0; j < nnodes; j++){
            cout << A[i][j] << "  ";
        }
        cout << endl;
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


// obtain the largest clique
vector<string> Network::clique(){
    //sort by degree
    vector<Node> nodes;
    for(int i = 0; i < nnodes; i++)
        nodes.push_back(Node(index[i], i));
    //lambda function to sort by degree
    sort(nodes.begin(), nodes.end(), [](Node a, Node b){ return a.deg > b.deg; });
    //remove nodes with lower degree
    
    return vector<string>();
}


int main() {
    Network nx;
    nx.load_nodes("test.txt");
    nx.print();
    //obtain the larges clique
    vector<string> clq = nx.clique();
    //short alphabetically for the solution
    sort(clq.begin(), clq.end());
    cout << "Clique: " << endl;
    for(string node : clq)
        cout << node << ",";
    cout << endl;
    return 0;
}