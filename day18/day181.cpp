/*
advent of code day 18 (1)
shortest path in la laberyth (traditional way)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; //up, right, down, left
int dc[4] = {0, 1, 0, -1};



class Laberynth{
    private:    
        vector<string> map;
        vector<vector<int> > M; //memoization
        size_t size;
    public:
        Laberynth(string filename, int size, int corrupted);
        void print();
        int solve();
    private:
        int shortest_path(int row, int col);
        bool inside(int row, int col);
};


Laberynth::Laberynth(string filename, int size, int corrupted) {
    fstream inputf(filename);
    string line;
    int row, col;
    //initialize map and memoization matrix
    map.resize(size);
    M.resize(size);
    for ( int i = 0 ; i < size ; i++ ){
        M[i].resize(size, 0);
        map[i].resize(size, '.');
    }
    //load matrix
    for(int i = 0; i < corrupted; i++){
        getline(inputf, line);
        sscanf(line.c_str(), "%d,%d", &col, &row);
        M[row][col] = '-1';
        map[row][col] = '#';
    }
    inputf.close();
    this->size = map.size();
}


void Laberynth::print() {
    for(string s: map)
        cout << s << endl;
    cout << "----------------" << endl;
}


bool Laberynth::inside(int row, int col){
    return row >= 0 && row < size && col >= 0 && col < size;
}


//shortest path from (0,0) to (n,n) using bread first search
int Laberynth::shortest_path(int row, int col){
    struct cell{
        int row, col, level;
        cell(int r, int c, int l): row(r), col(c), level(l) {}
    };
    struct lower_cost{
        bool operator()(cell& c1, cell& c2)
        {return c1.level > c2.level && c1.row > c2.row && c1.col > c2.col;}
    };
    priority_queue<cell, vector<cell>, lower_cost> q;
    q.push(cell(row, col,0));
    while(!q.empty()){
        //print();
        int row = q.top().row;
        int col = q.top().col;
        int level = q.top().level;
        q.pop();
        map[row][col] = 'o';
        if(row == size-1 && col == size-1) return level;
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(inside(r,c) && map[r][c] == '.'){
                q.push(cell(r, c, level+1));
            }
        }
    }
    return -1;
}

int Laberynth::solve(){
    int len = shortest_path(0, 0);
    cout << "shortest path: " << len << endl;
    return 0;
}


int main() {
    Laberynth lab("input.txt", 71, 1024);
    //Laberynth lab("test.txt", 7, 12);
    lab.print();
    lab.solve();
    return 0;
}

