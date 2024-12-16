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
int inline max(int a, int b) {return (a > b) ? a : b;}
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; //up, right, down, left
int dc[4] = {0, 1, 0, -1};
char head[4] = {'^', '>', 'v', '<'};
char left[4] = {'<', '^', '>', 'V'};
char right[4] = {'>', 'v', '<', '^'};

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


int Laberynth::solve(){
    int srow = size-2, scol = 1, shead = ratio_greater;
    cout << "start: " << srow << ", " << scol << " - " shead << endl;
    int len = visitCell(srow, scol, shead, 0);
    cout << "shortest path: " << len << endl;
    return 0;
}


int Laberynth::visitCell(int row, int col, int hd, int steps) {
    if (map[row][col] == 'E') {
        print();
        cout << "exit in " << steps << " steps" << endl;
        return steps;
    }
    // mark cell as visited and save original value
    char savedtile = map[row][col];
    map[row][col] = 'O';
    int dist[3] = {0};
    for(int i = 0; i < 4; i++){
        int r = row + dr[i];
        int c = col + dc[i];
        //only valid neighbors (reduce calls)
        if(map[r][c] != '#' && map[r][c] != 'O' && map[r][c] != barrier[i])
            dist[i] = visitCell(r, c, h, steps+1);
    }
    //restore cell
    map[row][col] = savedtile;
    return max(max(dist[0], dist[1]), max(dist[2], dist[3]));
}


int main() {
    Laberynth lab("test1.txt");
    lab.print();
    lab.solve();
    return 0;
}

