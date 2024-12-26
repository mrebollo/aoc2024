/*
advent of code day 6 (2)
calculate the possible positions for an obstacle
that forces the robot to get stuck on a loop
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

struct pos{
	int x, y, h;
};


//up, right, down, left
#define EMPTY 	0b000000
#define WALL 	0b010000
#define BARRIER 0b100000
#define UP 		0b000001
#define RG 		0b000010
#define DW 		0b000100
#define LF 		0b001000
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};
int dir[4] = {UP, RG, DW, LF};
char symbol[] = {'.','^','>','+','v','$','+','7','<','+','=','11','+'};

//loads the map into a string vector
vector<vector<int> > load(string filename){
	vector<vector<int> > map;
    fstream inputf(filename);
    string line;
    while(getline(inputf, line)){
		vector<int> row;
		for(int i = 0; i < line.size(); i++){
			if(line[i] == '#')
				row.push_back(WALL);
			else if(line[i] == '^')
				row.push_back(UP);
			else
				row.push_back(0);
		}
		map.push_back(row);
    }
    inputf.close();
    return map;
}

//finds the initial position marked with UP
void findStart(vector<vector<int> > &map, int&x, int &y){
	for( x = 0; x< map.size(); x++){
		auto it = find(map[x].begin(), map[x].end(), UP);
		if(it != map[x].end()){
			y = it - map[x].begin();
			map[x][y] = EMPTY;
			return;
		}
	}
}

// head rotates between up, right, down, left (0,1,2,3)
inline void turn(int &head){
	head = (head+1) % 4;
}

//check if (r,c) coordinates are inside the map
inline int inside(vector<vector<int> > &map, int r, int c){
	return r >=0 && r < map.size() && c >= 0 && c < map.size();
}

// returns the element in the next cell ahead, '.' if in border
int ahead(vector<vector<int> > &map, int r, int c, int h){
	if(inside(map, r+dr[h], c+dc[h]))
		return map[r+dr[h]][c+dc[h]];
	return -1;
	//return inside(map, r, c) ? map[r+dr[h], c+dc[h]] : '.';
}

// coordinates of the next cell following the heading
inline void next(int &r, int &c, int h){
	r += dr[h];
	c += dc[h];
}


//moves through the map until it moves away 
// using final recursion scheme
bool move(vector<vector<int> > &map, int r, int c, int h, int &steps, vector<pos> *path=NULL){
	//it moves away, terminates
	if(!inside(map, r, c))
		return true;
	//if already visited with same heading, terminates
	if(map[r][c] != EMPTY && map[r][c] & dir[h]){
		cout << r << "," << c << "["<< h << "] - visited " << bitset<8>(map[r][c]) << endl;
		return false;
	}
	//mark and count visited
	if(path && map[r][c] == EMPTY){ 
		//saves visited in the path
		pos p = {r,c,h};
		path->push_back(p);
	}
	//mark de cell as visited
	map[r][c] += dir[h];
	steps++;
	//turn when collision
	while(ahead(map, r, c, h) == WALL){
		turn(h);
		map[r][c] += dir[h];
	}
	//moves to the next cell following the current direction
	next(r, c, h);
	return move(map, r, c, h, steps, path);
}


void print(vector<vector<int> > &map){
	for(int i = 0; i < map.size(); i++){
		cout << to_string(i) << ":\t";
		for(int j = 0; j < map.size(); j++)
			if(map[i][j] == WALL)
				cout << '#';
			else if(map[i][j] == EMPTY)
				cout << '.';
			else
				cout << (char)('0' + map[i][j] );
		cout << endl;
	}
}


//cleans the breadcums of the path
void clean_map(vector<vector<int> > &map){
	for(int i = 0; i < map.size(); i++)
		for(int j = 0; j < map.size(); j++)
			if(map[i][j] != WALL)
				map[i][j] = EMPTY;
}


//count possible coordinates for obstacles
// that force the robot to get stuck on a loop
int add_obstacles(vector<vector<int> > &map, vector<pos> *path){
	int cicles = 0;
	for(int i = 1; i < path->size(); i++){
		//cleans the map
		clean_map(map);
		//puts an obstacle in position i, robot in i-1
		//and checks if it generates a loop
		//(not needed cover path from initial position)
		map[(*path)[i].x][(*path)[i].y] = WALL;
		int steps = 0;
		bool loop = move(map, (*path)[0].x, (*path)[0].y, (*path)[0].h, steps);
		if(!loop){
			cout << "loop in: " << i << endl;
			cicles++;
		}
		//print(map);
		//cout << "----------" << endl;
		//restores the map
		map[(*path)[i].x][(*path)[i].y] = EMPTY;

	}
	return cicles;
}


int main(){
	vector<vector<int> > map;
	vector<pos> path;
	int x, y, cells = 0, head = 0;
	map = load("input.txt");
	findStart(map, x, y);
	move(map, x, y, 0, cells, &path);
	print(map);
	cout << "len: " << cells << endl;
	cout << "unique: " << path.size() << endl;
	int cicles = add_obstacles(map, &path);
	cout << "cicles: " << cicles << endl;

	return 0;
}