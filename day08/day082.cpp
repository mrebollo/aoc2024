/*
advent of code day 8 (2)
mark points over the rect that pass through two points
grid -> manhattan distance can be used
antinodes repeat along all the line 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct coord{
    int x, y;
};

// load the positions of the antennas
void load(string filename, vector<vector<coord> > &ant, vector<string> &map){
    fstream file(filename);
    string line;
    int i = 0;
    while(getline(file, line)){
        map.push_back(line);
        for(int j = 0; j < line.size(); j++){
            //antenna found
            if(line[j] != '.'){
                //gets the coordinates of the antenna
                coord c = {i, j};
                //and adds to the list of the corresponding letter
                ant[line[j]].push_back(c);
            }
        }
        i++;
    }
    file.close();
}

//check if the coordinates are inside the map
inline bool inside(coord c, vector<string> &map){
    return c.x >= 0 && c.x < map.size() && c.y >= 0 && c.y < map[0].size();
}

//obtain the next point in the line
coord nextpoint(coord c, int distx, int disty){
    c.x += distx;
    c.y += disty;
    return c;
}

//obtain antinodes for a given antenna
int antinodes_for(vector<coord> &ant, vector<string> &map){
    int count = 0;
    // TODO: create methods -> coord class
    coord dist, n1, n2;

    for(int i = 0; i < ant.size()-1; i++){
        for(int j = i+1; j < ant.size(); j++){
            //get the manhattan distance between the two antennas in x and y
            dist.x = ant[j].x - ant[i].x;
            dist.y = ant[j].y - ant[i].y;
            //iterated antinode coordinates
            n1 = nextpoint(ant[i], -dist.x, -dist.y);
            while(inside(n1, map)){
                //check just '.' Antennas added outise this method
                if(map[n1.x][n1.y] == '.'){
                    map[n1.x][n1.y] = '#';
                    count++;
                }
                n1 = nextpoint(n1, -dist.x, -dist.y);
            }
            n2 = nextpoint(ant[j], dist.x, dist.y);
            while(inside(n2, map)){
                if(map[n2.x][n2.y] == '.'){
                    map[n2.x][n2.y] = '#';
                    count++;
                }
                n2 = nextpoint(n2, dist.x, dist.y);
            }
        }
    }
    return count;
}


// obtain antinodes for all antennas
int antinodes(vector<vector<coord> > &ant, vector<string> &map){
    int total = 0;
    vector<string> anti = map;
    for(int i = '0'; i < 'z'; i++){
        if(ant[i].size() < 2) continue;
        int count = antinodes_for(ant[i], map);
        cout << "Antenna: " << (char)i << " - nodes: " << count << endl;
        //antinodes occur also on antennas
        total += count + ant[i].size();
    }
    return total;
}


//print the map
void print(vector<string> &map){
    for(int i = 0; i < map.size(); i++){
        cout << map[i] << endl;
    }
}

int main(){
    // map with antennas (only '0'-'9', 'A'-'Z', 'a'-'z' used)
    vector<vector<coord> > ant(128);
    // map with the grid
    vector<string> map;
    // load the map
    load("input.txt", ant, map);
    // colocate antinodes
    int total = antinodes(ant, map);
    //print(map);
    cout << "Antinodes: " << total << endl;
     return 0;

}
