/*
advent of code day 6 (1)
calculate the possible positions for ann obstacle
that forces the robot to get stuck on a loop
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct pos{
	int x, y, h;
};

// saves the heading in each cell
// it's a loop if cell visited with the same heading
vector<vector<int> > memdir;

bool in_loop(vector<string> &map, int r, int c, int h){
	return map[r][c] == 'x' && memdir[r][c] == h;
}

//up, right, down, left
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};

//loads the map into a string vector
vector<string> load(string filename){
	vector<string> map;
    fstream inputf(filename);
    string line;
    while(getline(inputf, line)){
        map.push_back(line);
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
inline int inside(vector<string> &map, int r, int c){
	return r >=0 && r < map.size() && c >= 0 && c < map.size();
}

// returns the element in the next cell ahead, '.' if in border
char ahead(vector<string> &map, int r, int c, int h){
	if(inside(map, r+dr[h], c+dc[h]))
		return map[r+dr[h]][c+dc[h]];
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
void move(vector<string> &map, int r, int c, int h, int &steps, vector<pos> &path){
	//it moves away, terminates
	if(!inside(map, r, c))
		return;
	//mark and count visited
	if(map[r][c] != '#'){
		// saves heading to detect loops
		memdir[r][c] = h;
		//saves visited in the path
		if(map[r][c] != 'x'){
			pos p = {r,c};
			path.push_back(p);
		}
		map[r][c] = 'x';
		steps++;
	}
	//turn when collision
	if(ahead(map, r, c, h) == '#')
		turn(h);
	//moves to the next cell following the current direction
	next(r, c, h);
	move(map, r, c, h, steps, path);
}

void print(vector<string> &map){
	for(int i = 0; i < map.size(); i++)
		cout << map[i] << endl;
}


//the path from current position generates a loop
bool is_loop(vector<string> &map, int r, int c, int h){
	if(!inside(map, r, c))
		return false;
	if(in_loop(map, r, c, h))
		return true;
	//turn when collision
	if(ahead(map, r, c, h) == '#')
		turn(h);
	//moves to the next cell following the current direction
	next(r, c, h);
	return is_loop(map, r, c, h);
}

//check if obstacle in candidate position generates loop
bool create_loop(vector<string> &map, pos obs, pos start){
	bool res = false;
	//puts an obstacle in obs
	map[obs.x][obs.y] = '#';
	//begins at previous position
	int r = start.x, c = start.y, h = memdir[r][c];
	//forces hit new obstacle (turn)
	turn(h); 
	next(r, c, h);
	//moves until it moves away or loop detected
	if(is_loop(map, r, c, h))
		res = true;
	//restores the map
	map[obs.x][obs.y] = 'x';
	return res;

}


//count possible coordinates for obstacles
// that force the robot to get stuck on a loop
int add_obstacles(vector<string> &map, vector<pos> &path){
	int obstacles = 0;
	for(int i = 1; i < path.size(); i++)
		//puts an obstacle in position i, robot in i-1
		//and checks if it generates a loop
		//(not needed cover path from initial position)
		if(create_loop(map, path[i], path[i-1]))
			obstacles++;
	return obstacles;
}


int main(){
	vector<string> map;
	vector<pos> path;
	int x, y, cells = 0, head = UP;
	map = load("test.txt");
	memdir.resize(map.size(), vector<int>(map.size(), -1));
	findStart(map, x, y);
	move(map, x, y, head, cells, path);
	print(map);
	int obstacles = add_obstacles(map, path);
	cout << "obs: " << obstacles << endl;

}