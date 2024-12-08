#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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
void findStart(vector<string> map, int&x, int &y){
	for( x = 0; x< map.size(); x++){
		if(y = map[i].find("^") != npos)
			return;
}

// head rotates between up, right, down, left (0,1,2,3)
inline void turn(int &head){
	head = (head+1) % 4;
}

//check if (r,c) coordinates are inside the map
inline bool inside(vector<string> map, int r, int c){
	return r >=0 && r < map.size() && c >= 0 && c >= map.size();
}

// returns the element in the next cell ahead, '.' if in border
char ahead(vector<string> map, int &r, int &c, int &h){
	return inside(map, r, c) ? map[r+dr[h], c+dc[h]] : '.';
}

// coordinates of the next cell following the heading
void next(int &r, int &c, int &h){
	r += dr[h];
	c += dc[h];
}

//moves through the map until it moves away 
// using final recursion scheme
void move(vector<string> map, int &r, int &c, int &h, int &steps){
	//it moves away, terminates
	if(!inside(map, r, c))
		return;
	//mark and count visited
	if(map[r][c] != 'x'){
		map[r][c]. = 'x';
		steps++;
	}
	//turn when collision
	if(ahead(map, r, c, h) == '#')
		turn(h);
	//moves to the next cell following the current direction
	next(r, c, h);
	move(map, r, c, h, steps);
}

int main(){
	vector<string> map;
	int x, y, cells = 0, head = UP;
	map = load("test.txt");
	findStart(map, x, y);
	move(map, x, y, head, cells);
	cout << "cells: " << cells << endl;

}