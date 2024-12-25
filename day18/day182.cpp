/*
advent of code day 18 (2)
Detect new wall that disconnects the two ends

- Retrive the cells in the shortest path
- If the wall belongs to the path
    - remove it
    - find a path from the two ends (allow to go backwards through the path
*/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>

using namespace std;
//to solve backwards, better order
enum dir {UP, LF, DW, RG}; 
int dr[4] = {-1, 0, 1, 0}; //up, right, down, left
int dc[4] = {0, -1, 0, 1};


struct cell{
    int row, col, level;
    cell(): row(0), col(0), level(0) {}
    cell(int r, int c, int l): row(r), col(c), level(l) {}
    bool operator!=(const cell &b){
        return row != b.row || col != b.col;
    }
};

bool operator<(const cell &a, const cell &b){
    if(a.row != b.row)
        return a.row < b.row;
    if(a.col != b.col)
        return a.col < b.col;
    return a.level < b.level;
}

class Laberynth{
    private:    
        vector<string> mem;
        size_t size;
        map<cell, cell> parent;
        vector<cell> path;
    public:
        Laberynth(string filename, int size, int corrupted);
        void print(cell *mark);
        void print_path();
        void print_memo();
        int solve();
        string force_failure(string filename, int corrupted);
    private:
        int shortest_path(int row, int col);
        void retrieve_path(int len);
        bool is_connected() {return false;}
        inline bool inside(int row, int col){
            return row >= 0 && row < size && col >= 0 && col < size;
        }
};

// inits the mem loading the file
Laberynth::Laberynth(string filename, int size, int corrupted) {
    fstream inputf(filename);
    string line;
    int row, col;
    //initialize mem and memoization matrix
    mem.resize(size);
    for ( int i = 0 ; i < size ; i++ ){
        mem[i].resize(size, '.');
    }
    //load matrix
    for(int i = 0; i < corrupted; i++){
        getline(inputf, line);
        sscanf(line.c_str(), "%d,%d", &col, &row);
        mem[row][col] = '#';
    }
    inputf.close();
    this->size = mem.size();
}


// print the mem
void Laberynth::print(cell *mark=nullptr){
    if(mark != nullptr)
        mem[mark->row][mark->col] = 'X';
    for(string s: mem)
        cout << s << endl;
    cout << "----------------" << endl;
    if(mark != nullptr)
        mem[mark->row][mark->col] = 'o';
}



//shortest path using DP beginning from both ends
int Laberynth::shortest_path(int row, int col){
    queue<cell> q;
    q.push(cell(row, col, 0));
    while(!q.empty()){
        cell top = q.front();
        q.pop();
        if(mem[top.row][top.col] == 'o')
            continue;
        mem[top.row][top.col] = 'o';
        if(inside(top.row,top.col) && top.row == 0 && top.col == 0)
            return top.level;   
        for(int i = 0; i < 4; i++){
            int r = top.row + dr[i];
            int c = top.col + dc[i];
            if(inside(r,c) && mem[r][c] == '.'){
                parent[cell(r,c, top.level+1)] = top;
                q.push(cell(r, c, top.level+1));
            }
        }
    }
    return -1;
}

void Laberynth::retrieve_path(int len){
    cell end = cell(size-1,size-1,0);
    cell start = cell(0,0,len);
    cell *mark = &start;
    path.push_back(*mark);
    while(*mark != end){
        mem[mark->row][mark->col] = 'X';
        mark = &parent[*mark];
        path.push_back(*mark);
    }
    path.push_back(*mark);
    mem[0][0] = 'X';
}

string Laberynth::force_failure(string filename, int corrupted){
    fstream input(filename);
    string line;
    int row, col;
    for(int i = 0; i < corrupted; i++)
        getline(input, line);
    do{

    }while(is_connected());
    input.close();
    return line;
}
    


int Laberynth::solve(){
    int len = shortest_path(size-1, size-1);
    cout << "shortest path: " << len << endl;
    retrieve_path(len);
    return 0;
}



int main() {
    Laberynth *lab;
    lab = new Laberynth("test.txt", 7, 12);
    //lab = new Laberynth("input.txt", 71, 1024);
    lab->solve();
    string failpos = lab->force_failure("test.txt", 13);
    //lab->force_failure("input.txt", 1025);
    lab->print();
    cout << "Failure at: " << failpos << endl;
    return 0;
}

