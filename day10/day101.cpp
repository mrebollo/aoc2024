/*
advent of code day 10 (1)
look for all possible paths from 0 to 9
gradient ascent
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
            map[i][j] = line[j] - '0';
        i++;
    }while(getline(inputf, line));
    inputf.close();
}

//prints the map
void print(vector<vector<int> > &map){
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++)
            cout << (map[i][j] > 0 ? map[i][j] : 0);
        cout << endl;
    } 
}   


//recursive function to find all possible paths from a starting point
int trailhead_from(int i, int j){
    //base case: if the point is 9, return 1
    if(map[i][j] == 9){
        print(map);
        cout << endl;
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
            if(map[i][j] == 0)
                total += trailhead_from(i,j);
    return total;
}

int main(){
    load("test74.txt");
    print(map);
    int total = hicking();
    cout << "Total: " << total << endl;
    return 0;
}