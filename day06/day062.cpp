/*
advent of code day 6 (2)
calculate the possible positions for an obstacle
that forces the robot to get stuck on a loop
use a 3D matrix to represent the directions as layers
because a loop must maintain the same direction (not just position)

Alternative: use bit operations to keep the direction instead of a 3D matrix
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct pos{
	int r, c, h;
};


//up, right, down, left
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};
char sdir[4] = {'^', '>', 'v', '<'};

//loads the map into a string vector
vector<vector<string> > load(string filename){
	vector<vector<string> > map(4);
    fstream inputf(filename);
    string line;
    while(getline(inputf, line)){
		for(int dir = 0; dir < 4; dir++)
        	map[dir].push_back(line);
    }
    inputf.close();
    return map;
}


//finds the initial position marked with ^
void findStart(vector<string> &map, int&x, int &y){
	for( x = 0; x< map.size(); x++)
		if((y = map[x].find("^")) != string::npos){
			//reset the initial position
			map[x][y] = '.';
			return;
		}
}

// head rotates between up, right, down, left (0,1,2,3)
inline void turn(int &head){
	head = (head+1) % 4;
}

//check if (r,c) coordinates are inside the map
inline int inside(vector<vector<string> > &map, int r, int c){
	return r >=0 && r < map[0].size() && c >= 0 && c < map[0].size();
}

// returns the element in the next cell ahead, '.' if in border
char ahead(vector<vector<string> >&map, int r, int c, int h){
	if(inside(map, r+dr[h], c+dc[h]))
		return map[h][r+dr[h]][c+dc[h]];
	return '.';
	//return inside(map, r, c) ? map[r+dr[h], c+dc[h]] : '.';
}

// coordinates of the next cell following the heading
inline void next(int &r, int &c, int h){
	r += dr[h];
	c += dc[h];
}


//moves through the map until it moves away 
// using final recursion scheme
void move(vector<vector<string> > &map, int r, int c, int h, int &steps, vector<pos> &path){
	//it moves away, terminates
	if(!inside(map, r, c))
		return;
	//mark and count as visited if not done yet
	if(map[0][r][c] != 'x' && map[1][r][c] != 'x' &&
		map[2][r][c] != 'x' && map[3][r][c] != 'x')
		steps++;

	//add to the path already visited points with new direction
	if(map[h][r][c] != '#'){
		map[h][r][c] = 'x';
		pos p = {r,c,h};
		path.push_back(p);
	}
	//turn when collision
	if(ahead(map, r, c, h) == '#')
		turn(h);
	//moves to the next cell following the current direction
	next(r, c, h);
	move(map, r, c, h, steps, path);
}

void print(vector<vector<string> > &map, vector<pos> &path){
	//load the map empty
	vector<string> m = map[0];
	for(int i = 0; i < m.size(); i++)
		for(int j = 0; j < m[i].size(); j++)
			for(int dir = 1; dir < 4; dir++)
				if(map[dir][i][j] != '.' && map[dir][i][j] != '#')
					m[i][j] = map[dir][i][j];
	//draw the path
	for(pos step : path)
		m[step.r][step.c] = sdir[step.h];
	//
	for(int i = 0; i < m.size(); i++)
		cout << m[i] << endl;
}


//the path from current position generates a loop
bool is_loop(vector<vector<string> >&map, pos p, pos obs){
	if(!inside(map, p.r, p.c))
		return false;
	if(ahead(map, p.r, p.c, p.h) == 'O' || map[p.h][p.r][p.c] != '.')
		//hits the obstacle twice or it's a revisited cell -> loop
		return true;
	//mark visited
	map[p.h][p.r][p.c] = sdir[p.h];
	if(ahead(map, p.r, p.c, p.h) == 'H'){
		//hits the obstacle once
		for(int d = 0; d < 4; d++)
			map[d][p.r+dr[p.h]][p.c+dc[p.h]] = 'O';
 		turn(p.h);
	}
	//turn when collision
	if(ahead(map, p.r, p.c, p.h) == '#')
		turn(p.h);
	//moves to the next cell following the current direction
	next(p.r, p.c, p.h);
	return is_loop(map, p, obs);
}

//check if obstacle in candidate position generates loop
// it works over its own map copy
bool create_loop(vector<vector<string> > map, vector<pos> &path, int obsid){
	bool res = false;
	pos obs = path[obsid];
	pos start = path[obsid-1];
	vector<pos> cycle;
	//puts an obstacle in obs
	for(int d = 0; d < 4; d++)
		map[d][obs.r][obs.c] = 'H';
	//begins at previous position
	int r = start.r, c = start.c, h = start.h;
	//moves until it moves away or loop detected
	if(is_loop(map, start, obs)){
		print(map, cycle); cout << endl;
		res = true;
	}
	//restores the map
	for(int d = 0; d < 4; d++)
		map[d][obs.r][obs.c] = '.';
	return res;

}


//count possible coordinates for obstacles
// that force the robot to get stuck on a loop
int add_obstacles(vector<vector<string> > &map, vector<pos> &path){
	int obstacles = 0;
	for(int i = 1; i < path.size(); i++){
		//puts an obstacle in position i, robot in i-1
		//and checks if it generates a loop
		//(not needed cover path from initial position)
		if(create_loop(map, path, i)){
			obstacles++;
			cout << "[" << obstacles << "] Obs " << i << " generates loop" << endl;
		}
	}
	return obstacles;
}


int main(){
	vector<vector<string> > map;
	vector<pos> path;
	int row, col, cells = 0, head = UP;
	string filename = "test.txt";
	map = load(filename);
	findStart(map[0], row, col);
	move(map, row, col, head, cells, path);
	print(map, path);
	cout << "Cells: " << cells << endl;
	
	int obstacles;
	map.clear();
	map = load(filename);
	obstacles = add_obstacles(map, path);
	cout << "obs: " << obstacles << endl;
	

	return 0;
}