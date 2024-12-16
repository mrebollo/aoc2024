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


class Laberynth{
    private:    
        vector<string> map;
        size_t size;
    public:
        Laberynth(string filename);
        void print();
        int solve();
    private:
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
    //simlifica la salida para que se vea mejor
    for(int i = 0; i < size; i++) {
        //cout << map[i] << endl;
        for(int j = 0; j < size; j++)
            if( map[i][j] == 'O')
                cout << 'o';
            else if (map[i][j] == '#')
                cout << '.';
            else if (map[i][j] == '.')
                cout << ' ';
            else
                cout << map[i][j];
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


int Laberynth::visitCell(int row, int col, int hd, int steps) {
    cout << "visiting: " << row << ", " << col << " - " << head[hd] << " - " << steps << endl;
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
    char savedtile = map[row][col];
    map[row][col] = 'O';
    int r, c;
    if(ah != '#' && ah != 'O'){
        r = row + dr[hd]; c = col + dc[hd];
        dist[0] =  visitCell(r, c, hd, steps+1);
    }
    if(lf != '#' && lf != 'O'){
        r = row + dr[turnleft[hd]]; c = col + dc[turnleft[hd]];
        dist[1] = 1000 + visitCell(row, col, turnleft[hd], steps+1);
    }
    if(rg != '#' && rg != 'O'){
        r = row + dr[turnright[hd]]; c = col + dc[turnright[hd]];
        dist[2] = 1000 + visitCell(row, col, turnright[hd], steps+1);
    }
     //restore cell
    map[row][col] = savedtile;
    return *min_element(dist.begin(), dist.end());

}



int Laberynth::solve(){
    int srow = size-2, scol = 1, shead = RG;
    cout << "start: " << srow << ", " << scol << " - " << shead << endl;
    int len = visitCell(srow, scol, shead, 0);
    cout << "shortest path: " << len << endl;
    return 0;
}


int main() {
    Laberynth lab("test1.txt");
    lab.print();
    lab.solve();
    return 0;
}

