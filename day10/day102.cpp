/*
advent of code day 10 (2)
look for all possible paths from 0 to 9
gradient ascent considering all posible paths to 9 
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// map as global variable -> TODO: insert it in a class
vector<vector<int> > map;
int dx[4] = {0,0,1,-1};
int dy[4] = {1,-1,0,0};

inline bool inside(int x, int y){
    return x >= 0 && x < map.size() && y >= 0 && y < map.size();
}

//loads the map into a string vector
void load(string filename){
    fstream inputf(filename);
    string line;
    //reads the first line to get the size of the map
    getline(inputf, line);
    map = vector<vector<int> >(line.size(), vector<int>(line.size(),0));
    int i = 0;
    do{
        for(int j = 0; j < line.size(); j++)
            //just for tests
            map[i][j] = line[j] == '.' ? -9 : line[j] - '0';
        i++;
    }while(getline(inputf, line));
    inputf.close();
}

//prints the map
void print(vector<vector<int> > &map){
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++)
            if(map[i][j] == -1)
                cout << "x";
            else if(map[i][j] == -9)
                cout << "."; 
            else
                cout << to_string(map[i][j]);
        cout << endl;
    } 
}   

//unmark the cells with a 9 that ended up being visited
void restore(){
    for(int i = 0; i < map.size(); i++)
        for(int j = 0; j < map[i].size(); j++)
            if(map[i][j] == -1)
                map[i][j] = 9;
}


//recursive function to find all possible paths from a starting point
int trailhead_from(int i, int j){
    //base case: if the point is 9, return 1
    if(map[i][j] == 9){
        //print(map);
        //cout << endl;
        //mark the point as visited (avoid alternative path to the same)
        //***line commented for part 2 ***
        //map[i][j] = -1;
        return 1;
    }
    //mark the point as visited
    int temp = map[i][j];
    map[i][j] = -1;
    int total = 0;
    //check all possible directions
    for(int dir = 0; dir < 4; dir++){
        int x = i + dx[dir];
        int y = j + dy[dir];
        //check inside map and next value
        if(inside(x,y) && map[x][y] == temp + 1)
            total += trailhead_from(x,y);
    }
    //unmark the point
    map[i][j] = temp;
    return total;
}

//obtain all possible hicking trails from 0 to 9
int hicking(){
    int total = 0;
    //find all the starting points (0)
    for(int i = 0; i < map.size(); i++)
        for(int j = 0; j < map.size(); j++)
            if(map[i][j] == 0){
                 cout << "Starting point: (" << i << ", " << j << ")" << endl;
                int paths = trailhead_from(i,j);
                cout << "trailheads found: " << paths << endl;
                total += paths;
                //unmark the sucessful paths
                //***line commented for part 2 ***
                //restore();
            }
    return total;
}

int main(){
    load("input.txt");
    print(map);
    int total = hicking();
    cout << "Total: " << total << endl;
    return 0;
}