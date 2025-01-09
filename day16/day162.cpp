/*
advent of code day 16 (1)
shortest path in la laberyth
turn +90/-90 degrees when hitting a wall (cost +1000)

Compute all shortest paths and count visited cells
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>

using namespace std;
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; //up, right, down, left
int dc[4] = {0, 1, 0, -1};
char head[4] = {'^', '>', 'v', '<'};
int turnleft[4] = {LF, UP, RG, DW};
int turnright[4] = {RG, DW, LF, UP};
inline int min(int a, int b) { return (a < b) ? a : b; }


class Laberynth{
    private:    
        vector<string> map;
        size_t size;
    public:
        Laberynth(string filename);
        void print();
        int solve();
    private:
        int all_paths(int srow, int scol, int shead);
        int visitCell(int row, int col, int hd, int steps);
        char ahead(int row, int col, int hd);
        char ontheleft(int row, int col, int hd);
        char ontheright(int row, int col, int hd);
};


Laberynth::Laberynth(string filename) {
    fstream inputf(filename);
    string line;
    while(getline(inputf, line)){
        map.push_back(line);
    }
    inputf.close();
    size = map.size();
}


void Laberynth::print() {
    for(string &line: map){
        for(char &c: line)
            cout << ((c == '.') ? ' ' : c);
        cout << endl;
    }
}



char Laberynth::ahead(int row, int col, int hd){
    int r = row + dr[hd];
    int c = col + dc[hd];
    return map[r][c];
}

char Laberynth::ontheleft(int row, int col, int hd){
    int r = row + dr[turnleft[hd]];
    int c = col + dc[turnleft[hd]];
    return map[r][c];
}

char Laberynth::ontheright(int row, int col, int hd){
    int r = row + dr[turnright[hd]];
    int c = col + dc[turnright[hd]];
    return map[r][c];
}

/*
int Laberynth::visitCell(int srow, int scol, int hd, int steps) {
    struct cell{
        int row,col,hd,cost;
        cell(int r, int c, int h, int co): row(r), col(c), hd(h), cost(co){}
    };
    struct lower_cost{
        bool operator()(cell& c1, cell& c2)
        {return c1.cost > c2.cost;}
    };
    priority_queue<cell, vector<cell>, lower_cost> q;
    q.push(cell(srow, scol, hd, 0));
    while(!q.empty()){
        //print();
        cell pos = q.top();
        q.pop();
        if(map[pos.row][pos.col] == 'E') return pos.cost;
        map[pos.row][pos.col] = 'o';
        //ahead
        int r = pos.row + dr[pos.hd];
        int c = pos.col + dc[pos.hd];
        if(map[r][c] != '#' && map[r][c] != 'o'){
            q.push(cell(r, c, pos.hd, pos.cost+1));
        }
        //left
        r = pos.row + dr[turnleft[pos.hd]];
        c = pos.col + dc[turnleft[pos.hd]];
        if(map[r][c] != '#' && map[r][c] != 'o'){
            //just turn
            q.push(cell(pos.row, pos.col, turnleft[pos.hd], pos.cost+1000));
            //map[r][c] = 'o';
        }
        //right
        r = pos.row + dr[turnright[pos.hd]];
        c = pos.col + dc[turnright[pos.hd]];
        if(map[r][c] != '#' && map[r][c] != 'o'){
            //just turn
            q.push(cell(pos.row, pos.col, turnright[pos.hd], pos.cost+1000));
            //map[r][c] = 'o';
        }
    }
    return -1;

}
*/


int Laberynth::visitCell(int srow, int scol, int hd, int steps) {
    if(map[srow][scol] == 'E') 
        return steps;
    
    //ahead
    int r = srow + dr[hd];
    int c = scol + dc[hd];
    if(map[r][c] == '#' || map[r][c] == 'o')
        return 999999;
    map[srow][scol] = 'o';
    if(map[r][c] != '#' && map[r][c] != 'o')
        return visitCell(r, c, hd, steps) + 1;
    //left
    r = srow + dr[turnleft[hd]];
    c = scol + dc[turnleft[hd]];
    if(map[r][c] != '#' && map[r][c] != 'o')
        return visitCell(r, c, turnleft[hd], steps+1000) + 1000;
    //right
    r = srow + dr[turnright[hd]];
    c = scol + dc[turnright[hd]];
    if(map[r][c] != '#' && map[r][c] != 'o')
        return visitCell(r, c, turnright[hd], steps+1000) + 1000;

        

}

//shortest path from 'S' to 'E' using deep first search
int Laberynth::all_paths(int srow, int scol, int shead){
    struct cell{
        int row, col, len;
        cell(int r, int c, int l): row(r), col(c), len(l) {}
    };  
    queue<cell> q;
    q.push(cell(srow, scol, 0));
    while(!q.empty()){
        print();
        int row = q.front().row;
        int col = q.front().col;
        int level = q.front().len;
        q.pop();
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(map[r][c] == 'E') return level;
            if(map[r][c] == '.'){
                q.push(cell(r, c, level+1));
                map[r][c] = 'o';
            }
        }
    }
    return -1;
}


int Laberynth::solve(){
    int srow = size-2, scol = 1, shead = RG;
    cout << "start: " << srow << ", " << scol << " - " << shead << endl;
    map[srow][scol] = 'o';
    int len = visitCell(srow, scol, shead, 0);
    cout << "visite cells len: " << len << endl;
    //len = shortest_path(srow, scol, shead);
    //cout << "shortest path: " << len << endl;

    return 0;
}


int main() {
    Laberynth lab("test1.txt");
    lab.solve();
    lab.print();

    return 0;
}

