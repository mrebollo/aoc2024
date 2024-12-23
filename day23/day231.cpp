/*
    advent of code 23 (1)
    Count computers begining from with 't' that appears in a triade

    Local clustering coefficient
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
    Node (string n, int i): name(n), id(i) {};
};

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
                if(v.name == name)
                    return v.id;
            }
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
        int nodeid(string node){
            return node[0] - 'a';
        }
        string nodename (int id){
            return string(1, id + 'a');
        }
    public:
        //TODO: dynamic allocation
        Network(){nnodes = 0; A.resize(50, vector<int>(50, 0)); index.resize(50);};
        void load_nodes(string filename);
        void print();
        int clustering(Node u);
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
        index[uid] = u;
        index[vid] = v;
        //insert the edge in the adjacency matrix
        A[uid][vid] = 1;  A[vid][uid] = 1;
        nnodes = node_lst.size();
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
            for(int j = i+1; j < nnodes; j++)
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
        int Ci = nx.clustering(u);
        cout << "Node " << u.name << " - " << Ci << " triangles" << endl;
        triangles += Ci;
    }
    return triangles;
}


int main() {
    Network nx;
    nx.load_nodes("test.txt");
    nx.print();
    int triangles = count_triangles(nx);
    cout << "Triangles: " << triangles << endl;
    return 0;
}