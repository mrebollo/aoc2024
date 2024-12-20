/*
advent of code day 20 (1)
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
        Laberynth(string filename);
        void print(cell *mark);
        int solve();
    private:
        void findStart(vector<string> map, pair<int, int> &start, pair<int, int> &end);
        cell shortest_path(pair<int, int> &start, pair<int, int> &end);
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
Laberynth::Laberynth(string filename) {
    fstream inputf(filename);
    string line;
    while(getline(inputf, line))
        map.push_back(line);
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

//finds the initial and final positions marked with S and E
void Laberynth::findStart(vector<string> map, pair<int, int> &start, pair<int, int> &end){
	int x, y;
    for(x = 0; x< map.size(); x++)
		if((y = map[x].find("S")) != string::npos)
			start = make_pair(x, y);
        else if((y = map[x].find("E")) != string::npos)
            end = make_pair(x, y);
}


//shortest path using BFS. Exit when arrives to the plain
cell Laberynth::shortest_path(pair<int, int> &start, pair<int, int> &endl){
    pair<int, int> pos = start;
    int row, col, level = 0;
    while(pos != endl){ 
        row = pos.first;
        col = pos.second;
        map[row][col] = 'o';
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(map[r][c] == '.' || map[r][c] == 'E'){
                pos = make_pair(r, c);
                level++;
            }
        }
    }
    return cell(row, col, level);
}

int Laberynth::solve(){
    pair<int, int> start, end;
    findStart(map, start, end);
    cell sol = shortest_path(start, end);
    print();
    cout << "shortest path: " << sol.level << endl;
    return 0;
}


int main() {
    Laberynth lab("input.txt");
    lab.print();    
    lab.solve();
    return 0;
}

