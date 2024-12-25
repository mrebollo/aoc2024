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
//to solve backwards, better order
enum dir {UP, LF, DW, RG}; 
int dr[4] = {-1, 0, 1, 0}; //up, right, down, left
int dc[4] = {0, -1, 0, 1};


struct cell{
    int row, col, level;
    cell(int r, int c, int l): row(r), col(c), level(l) {}
};


class Laberynth{
    private:    
        vector<string> map;
        size_t size;
    public:
        Laberynth(string filename, int size, int corrupted);
        void print(cell *mark);
        void print_path();
        void print_memo();
        int solve();
    private:
        int shortest_path(int row, int col);
        inline bool inside(int row, int col){
            return row >= 0 && row < size && col >= 0 && col < size;
        }
        bool in_plain(int row, int col);
        inline int manhattan(cell c1, cell c2){
            return abs(c1.row - c2.row) + abs(c1.col - c2.col);
        }
        void add_path(cell top, cell bottom);
};

// inits the map loading the file
Laberynth::Laberynth(string filename, int size, int corrupted) {
    fstream inputf(filename);
    string line;
    int row, col;
    //initialize map and memoization matrix
    map.resize(size);
    for ( int i = 0 ; i < size ; i++ ){
        map[i].resize(size, '.');
        M.push_back(vector<int>(size, 0));
    }
    //load matrix
    for(int i = 0; i < corrupted; i++){
        getline(inputf, line);
        sscanf(line.c_str(), "%d,%d", &col, &row);
        map[row][col] = '#';
    }
    inputf.close();
    this->size = map.size();
}


// print the map
void Laberynth::print(cell *mark=nullptr){
    if(mark != nullptr)
        map[mark->row][mark->col] = 'X';
    for(string s: map)
        cout << s << endl;
    cout << "----------------" << endl;
    if(mark != nullptr)
        map[mark->row][mark->col] = 'o';
}



//shortest path using DP beginning from both ends
int Laberynth::shortest_path(int row, int col){
    queue<cell> q;
    q.push(cell(row, col, 0));
    while(!q.empty()){
        cell top = q.front();
        q.pop();
        if(map[top.row][top.col] == 'o')
            continue;
        if(inside(top.row,top.col) && top.row == 0 && top.col == 0)
            return top.level;
        map[top.row][top.col] = 'o';
        for(int i = 0; i < 4; i++){
            int r = top.row + dr[i];
            int c = top.col + dc[i];
            if(inside(r,c) && map[r][c] == '.'){
                q.push(cell(r, c, top.level+1));
            }
        }
    }
    return -1;
}

int Laberynth::solve(){
    int len = shortest_path(M, size-1, size-1);
    cout << "shortest path: " << len << endl;
    return 0;
}


int main() {
    Laberynth *lab;
    //lab = new Laberynth("test.txt", 7, 12);
    lab = new Laberynth("input.txt", 71, 1024);
    lab->solve();
    lab->print();
    return 0;
}

