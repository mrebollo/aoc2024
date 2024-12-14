/*
advent of code day 14 (2)
count movents until a christmas tree shape appears
(eastern egg)
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
    robot operator+=(int t){
        x = abs((x+(vx * t)+MAXX) % MAXX);
        y = abs((y+(vy * t)+MAXY) % MAXY);
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
void plot(vector<robot> &rob){
    int grid[MAXX][MAXY] = {0};
    for(robot r: rob)
        grid[r.x][r.y]++;
    for(int i = 0; i < MAXY; i++){
        for(int j = 0; j < MAXX; j++)
            if (grid[j][i] == 0)
                cout << " ";
            else
                cout << "#";
        cout << endl;
    }   
}


// move robots t times 
void move(vector<robot> &rob, int t){
    for(robot &r : rob)
        // review why it is not working
        //r += t;
        for(int i = 0; i < t; i++)
            r += 1;
}   


// load the input file
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


//save the current positions in a pgm bitmap
void save(vector<robot> &rob, int t){
    char filename[100];
    sprintf(filename, "../../../map/d14_%04d.pgm",t);
    fstream output(filename, ios::app);
    output << "P2" << endl;
    output << MAXY << " " << MAXX << endl;
    output << "255" << endl;
    vector<vector<int> > grid(MAXX, vector<int>(MAXY, 0));
    for(robot r: rob)
        grid[r.x][r.y]++;
    for(int i = 0; i < MAXY; i++){
        for(int j = 0; j < MAXX; j++)
            output << (grid[j][i] == 0 ? 255 : 0) << " ";
        output << endl;
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
    //generates pictures to examinate the files 
    for(int t = 1; t < 10000; t++){
        move(rob,1);
        save(rob, t);
        cout << "Time: " << t << endl;
    }
    return 0;
}

