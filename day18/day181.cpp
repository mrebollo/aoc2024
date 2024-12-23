/*
advent of code day 18 (1)
shortest path in la laberyth (traditional way)

The are two laberynths in the corners, and a plain area in between
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
int diagr[4] = {-1, -1, 1, 1}; // nw, ne, sw, se
int diagc[4] = {-1, 1, -1, 1};


struct cell{
    int row, col, level;
    cell(int r, int c, int l): row(r), col(c), level(l) {}
};

struct lower_cost{
    bool operator()(cell& c1, cell& c2)
    {return c1.level > c2.level && c1.row > c2.row && c1.col > c2.col;}
};


class Laberynth{
    private:    
        vector<string> map;
        size_t size;
    public:
        Laberynth(string filename, int size, int corrupted);
        void print(cell *mark);
        int solve();
    private:
        int shortest_path(vector<vector<int> >& M, int row, int col);
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


// add an straight path from top to bottom 
void Laberynth::add_path(cell top, cell bottom){
    for(int i = top.row; i <= bottom.row; i++)
        map[i][top.col] = 'o';
    for(int i = top.col; i <= bottom.col; i++)
        map[bottom.row][i] = 'o';
}



//shortest path using BFS. Exit when arrives to the plain
int Laberynth::shortest_path(vector<vector<int> >& M, int row, int col){
    int len;
    if(row == 0 && col == 0) 
        return 0;
    map[row][col] = 'o';
    int min_len = 9999;
    for(int i = 0; i < 4; i++){
        int r = row + dr[i];
        int c = col + dc[i];
        if(inside(r,c) && map[r][c] == '.' && M[r][c] == 'x'){
            len = min(len, shortest_path(M, r, c) + 1);
        }
    }
    map[row][col] = 'x';
    if(len < min_len)
        return len;
    return 9999;
}


int Laberynth::solve(){
    vector<vector<int> > M(size, vector<int>(size, 0));
    print();
    int len = shortest_path(M, size-1, size-1);
    print();
    cout << "shortest path: " << len << endl;
    return 0;
}


int main() {
    //Laberynth lab("input.txt", 71, 1024);
    Laberynth lab("test.txt", 7, 12);
    lab.solve();
    return 0;
}

