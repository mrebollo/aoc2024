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
        vector<vector<int> > map;
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
    map.resize(size);
    for ( int i = 0 ; i < size ; i++ )
        map[i].resize(size, 0);
    for(int i = 0; i < corrupted; i++){
        getline(inputf, line);
        sscanf(line.c_str(), "%d,%d", &col, &row);
        map[row][col] = -1;
    }
    inputf.close();
    this->size = map.size();
}


void Laberynth::print() {
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++)
            if (map[i][j] == 0) cout << '.';
            else if (map[i][j] == -1) cout << '#';
            else cout << 'o';
        cout << endl;
    }
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
        {return c1.level > c2.level;}
    };
    priority_queue<cell, vector<cell>, lower_cost> q;
    q.push(cell(row, col,0));
    while(!q.empty()){
        //print();
        int row = q.top().row;
        int col = q.top().col;
        int level = q.top().level;
        q.pop();
        map[row][col] = level;
        if(row == size-1 && col == size-1) return level;
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(inside(r,c) && map[r][c] == 0){
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
    Laberynth lab("test.txt", 71, 1024);
    //Laberynth lab("test.txt", 7, 12);
    //lab.print();
    lab.solve();
    return 0;
}

