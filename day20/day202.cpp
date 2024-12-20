/*
advent of code day 20 (2)
shortest path in la laberyth (traditional way)

extend distance to 20 steps
solve with manhattan distances between all cells in the path,
ignoring the walls (valid also for part 1)
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

#define MAXDIST 20

struct cell{
    int row, col, level;
    cell(int r, int c, int l): row(r), col(c), level(l) {}
};


class Laberynth{
    private:    
        vector<string> map;
        size_t size;
        vector<cell> path;
    public:
        Laberynth(string filename);
        void print(cell *mark);
        int solve();
    private:
        void findStart(vector<string> map, pair<int, int> &start, pair<int, int> &end);
        cell shortest_path(pair<int, int> &start, pair<int, int> &end);
        inline int manhattan(cell c1, cell c2){
            return abs(c1.row - c2.row) + abs(c1.col - c2.col);
        }
        void add_to_path(cell c){ path.push_back(c); }
        vector<int> savings();
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
        add_to_path(cell(row, col, level));
        for(int i = 0; i < 4; i++){
            int r = row + dr[i];
            int c = col + dc[i];
            if(map[r][c] == '.' || map[r][c] == 'E'){
                pos = make_pair(r, c);
                level++;
            }
        }
    }
    map[row][col] = 'o';
    return cell(row, col, level);
}



// obtain the savings of removing all possible walls
vector<int> Laberynth::savings(){
    vector<int> pico(100, 0);
    for(int i = 0; i < path.size(); i++){
        for(int j = i+1; j < path.size(); j++){
            int dist = manhattan(path[i], path[j]);
            int realdist = abs(path[i].level - path[j].level);
            if(dist < realdist && dist <= MAXDIST)
                pico[realdist-2]++;
        }
    }
    return pico;
}



int Laberynth::solve(){
    pair<int, int> start, end;
    findStart(map, start, end);
    cell sol = shortest_path(start, end);
    print();
    cout << "shortest path: " << sol.level << endl;
    vector<int> pico = savings();
    int total = 0;
    for(int i = 50; i < pico.size(); i++){
        if(pico[i] == 0) continue;
        cout << i << " picoseconds: " << pico[i] << endl;
        total += pico[i];
    }
    return total;
}


int main() {
    Laberynth lab("test.txt");
    lab.print();    
    int total = lab.solve();
    cout << "total: " << total << endl;
    return 0;
}

