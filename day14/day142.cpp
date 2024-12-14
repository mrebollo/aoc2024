/*
advent of code day 14 (1)
count movents until in christmas tree shape
ej
...1...
..1.1..
.1...1.
1.....1
...1...
fist and last in center
next = (x+1, y +/- 1)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define TIME 100
//#define _TEST 0
#ifdef _TEST
    #define MAXX 11 
    #define MAXY 7 
    #define CX 5 
    #define CY 3 
#else
    #define MAXX 101
    #define MAXY 103
    #define CX 50
    #define CY 51
#endif
using namespace std;

struct robot{
    int x, y, vx, vy;
    string show(){
        return "p(" + to_string(x) + "," + to_string(y) + ") v(" + to_string(vx) + "," + to_string(vy) + ")";
    }
    bool operator ==(const robot &b){
        return x == b.x && y == b.y;
    }
    bool operator <(const robot &b) const{
        return x < b.x || (x == b.x && y < b.y);
    }
    robot operator+=(int t){
        x = abs((x+(vx * t)+MAXX) % MAXX);
        y = abs((y+(vy * t)+MAXY) % MAXY);
        return *this;
    }
    robot operator-=(int t){
        x = abs((x-(vx * t)+MAXX) % MAXX);
        y = abs((y-(vy * t)+MAXY) % MAXY);
        return *this;
    }
};

int quadrant(robot r){
    if(r.x < CX && r.y < CY) return 0;
    if(r.x > CX && r.y < CY) return 1;
    if(r.x < CX && r.y > CY) return 2;
    if(r.x > CX && r.y > CY) return 3;
    return -1;
}


int safety(vector<robot> &rob){
    int count[4] = {0};
    for(robot r : rob){
        int q = quadrant(r);
        if (q >= 0) count[quadrant(r)]++;
    }
    for(int c : count)
        cout << c << " ";
    cout << endl;
    return count[0] * count[1] * count[2] * count[3];
}

//show robots' positions
void plot(vector<robot> &rob, robot mark){
    int grid[MAXX][MAXY] = {0};
    for(robot r
    
    : rob)
        grid[r.x][r.y]++;
    if(mark.x != -1){
        mark += 1;
        grid[mark.x][mark.y] = 'x';
    }
    for(int i = 0; i < MAXY; i++){
        for(int j = 0; j < MAXX; j++)
            if (grid[j][i] == 0)
                cout << " ";
            else
                cout << "#";
        cout << endl;
    }   
}

//move robots t times forward
void fwd(vector<robot> &rob, int t){
    for(robot &r : rob)
        r += t;
}

//move robots t times backward
void bkwd(vector<robot> &rob, int t){
    for(robot &r : rob)
        r -= t;
}


vector<robot> move(vector<robot> &rob, int t){
    if(t > 0)
        fwd(rob, t);
    else
        bkwd(rob, t);
    vector<robot> copy = rob;
    return copy;
}   


void load(vector<robot> &rob, string filename){
    fstream input(filename);
    string line;
    robot r;
    while(getline(input, line)){
        // format: "p=0,4 v=3,-3"
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &r.x, &r.y, &r.vx, &r.vy);
        rob.push_back(r);
    }
    input.close();
}

bool is_tree(vector<robot> &rob, int id, int row){
    if(row == MAXY-1)
        return true;
    if(rob[id].x != row)
        return false;
    if(rob[id].y == CY-id) 
        return is_tree(rob, ++id, row);
    if(rob[id].y == CY+id)
        return is_tree(rob, ++id, ++row);
    return false;
}


int move_until_tree(vector<robot> &rob){
    for(int t = 1; ; t++){
        vector<robot> step = move(rob, 1);
        sort(rob.begin(), rob.end());
        if(rob[0].x > 0 || rob[0].y != CY || rob[rob.size()-1].x < MAXX || rob[rob.size()-1].y != CY)
            continue;
        if(is_tree(step, 1, 0))
            return t;
    }
}

void save(vector<robot> &rob, int t){
    char filename[100];
    sprintf(filename, "~/Desktop/map/d14_%04d.pgm",t);
    fstream output(filename, ios::app);
    output << "P5" << endl;
    output << MAXX << " " << MAXY << endl;
    output << "255" << endl;
    int grid[MAXX][MAXY] = {0};
    for(robot r: rob)
        grid[r.x][r.y]++;
    for(int i = 0; i < MAXY; i++){
        for(int j = 0; j < MAXX; j++)
            output << (grid[j][i] == 0 ? 255 : 0);
        cout << endl;
    }  
    output.close();
}


int main(){
    vector<robot> rob;
    robot nomark = {-1,-1,0,0};
    //load the input
#ifdef _TEST
    load(rob, "test.txt");
#else
    load(rob, "input.txt");
#endif
    save(rob, 0);
    for(int t = 1; t < 10000; t++){
        move(rob,1);
        save(rob, t);
        cout << "Time: " << t << endl;
    }
    return 0;
}

