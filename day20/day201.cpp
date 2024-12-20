/*
advent of code day 20 (1)
shortest path in a laberyth removing walls

It's mnot needed to recalculate: difference between the extremes
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
        inline bool inside(int row, int col){
            return row >= 0 && row < size && col >= 0 && col < size;
        }
        void add_to_path(cell c){ path.push_back(c); }
        vector<cell> vertical_removable();
        vector<cell> horizontal_removable();
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

// obtain walls that can be vertically removed (connect free cells '.')
vector<cell> Laberynth::vertical_removable(){
    vector<cell> removable;
    for(int i = 1; i < size-1; i++){
        for(int j = 1; j < size-1; j++){
            if(map[i][j] == '#'){
                if(inside(i-1, j) && inside(i+1, j) && map[i-1][j] == 'o' && map[i+1][j] == 'o')
                    removable.push_back(cell(i, j, 0));
            }
        }
    }
    return removable;
}

// obtain walls that can be horizontally removed (connect free cells '.')
vector<cell> Laberynth::horizontal_removable(){
    vector<cell> removable;
    for(int i = 1; i < size-1; i++){
        for(int j = 1; j < size-1; j++){
            if(map[i][j] == '#'){
                if(inside(i, j-1) && inside(i, j+1) && map[i][j-1] == 'o' && map[i][j+1] == 'o')
                    removable.push_back(cell(i, j, 0));
            }
        }
    }
    return removable;
}

// obtain the savings of removing all possible walls
vector<int> Laberynth::savings(){
    vector<int> pico(9999, 0);
    // create a matrix to store the distances to the end of the path
    vector<vector<int> > d(size, vector<int>(size, 0));
    for(cell step: path)
        d[step.row][step.col] = step.level;
    // obtain all possible removable walls
    vector<cell> vrem = vertical_removable();
    vector<cell> hrem = horizontal_removable();
    // TODO: define a function for both cases
    // remove vertical walls and recalculate distance
    int minstep, maxstep;
    for(cell wall: vrem){
        // sum the distances between the connected cells
        // vertical wall -> connect row-1 and row+1
        // - take steps in both cells
        int dist = abs(d[wall.row-1][wall.col] - d[wall.row+1][wall.col]);
        // reduce the path distance its difference
        pico[dist-2]++;
    }
    // same for horizontal walls
    for(cell wall: hrem){
        int dist = abs(d[wall.row][wall.col-1] - d[wall.row][wall.col+1]);
        pico[dist-2]++;
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
    for(int i = 100; i < pico.size(); i++){
        if(pico[i] == 0) continue;
        cout << i << " picoseconds: " << pico[i] << endl;
        total += pico[i];
    }
    return total;
}


int main() {
    Laberynth lab("input.txt");
    lab.print();    
    int total = lab.solve();
    cout << "total: " << total << endl;
    return 0;
}

