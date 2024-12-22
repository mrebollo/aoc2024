/*
advent of code day 16 (1)
shortest path in la laberyth
turn +90/-90 degrees when hitting a wall (cost +1000)
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
        int shortest_path(int srow, int scol, int shead);
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
int Laberynth::visitCell(int row, int col, int hd, int steps) {
    if(steps > 60) exit(0);
    print();
    cout << "visiting: [" << steps << "] " << row << ", " << col << " " << head[hd] << endl;
    if (map[row][col] == 'E') {
        print();
        cout << "exit in " << steps << " steps" << endl;
        return steps;
    }
    // mark cell as visited and save original value
    vector<int> dist(3, RAND_MAX);
    char lf = ontheleft(row, col, hd);
    char rg = ontheright(row, col, hd);
    char ah = ahead(row, col, hd);
    // save cell
    //char savedtile = map[row][col];
     map[row][col] = 'o';
    int r, c;
    if(ah != '#' && ah != 'o'){
        r = row + dr[hd]; c = col + dc[hd];
        dist[0] =  visitCell(r, c, hd, steps+1);
    }
    if(lf != '#' && lf != 'o'){
        r = row + dr[turnleft[hd]]; c = col + dc[turnleft[hd]];
        dist[1] = 1000 + visitCell(row, col, turnleft[hd], steps+1);
    }
    if(rg != '#' && rg != 'o'){
        r = row + dr[turnright[hd]]; c = col + dc[turnright[hd]];
        dist[2] = 1000 + visitCell(row, col, turnright[hd], steps+1);
    }
   
     //restore cell
    //map[row][col] = savedtile;
    return min(dist[0], min(dist[1], dist[2]));

}
*/

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


//shortest path from 'S' to 'E' using bread first search
int Laberynth::shortest_path(int srow, int scol, int shead){
    queue<int> steps;
    queue<pair<int, int> > q;
    q.push(make_pair(srow, scol));
    steps.push(0);
    while(!q.empty()){
        print();
        int row = q.front().first;
        int col = q.front().second;
        q.pop();
        int level = steps.front();
        steps.pop();
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(map[r][c] == 'E') return level;
            if(map[r][c] == '.'){
                q.push(make_pair(r, c));
                steps.push(level+1);
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

    //int len = shortest_path(srow, scol, shead);
    cout << "shortest path: " << len << endl;
    return 0;
}


int main() {
    Laberynth lab("input.txt");
    lab.print();
    lab.solve();
    return 0;
}

