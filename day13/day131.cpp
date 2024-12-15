/*
advent of code day 13 (1)
claws machine
move the claw to the target position to win the prize
two butons A, B with different speeds and costs (A 3tk, B 1tk)
max 100 movements
a typical PD problem
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <algorithm>

#define MAX_VALUE 99999
#define MAX_MOVES 100
#define TKN_A 3
#define TKN_B 1

using namespace std;

struct coor{
    int x, y;
    string show(){
        return "(" + to_string(x) + "," + to_string(y) + ")";
    }
    bool operator ==(const coor &b){
        return x == b.x && y == b.y;
    }
    bool operator < (const coor &b){
        return x < b.x || y < b.y;
    }
    coor operator -(const coor  &b){
        return {x - b.x, y - b.y};
    }
};
coor origin = {0, 0};


class Claws{
private:
    coor A, B, prize;
    int moves;
    long cost;
public:
    Claws(coor A, coor B, coor prize);
    int move_claw(vector<vector<int> > &M, coor pos, int nA, int nB);
    void show();
};


Claws::Claws(coor A, coor B, coor prize){
    this->A = A;
    this->B = B;
    this->prize = prize;
    moves = 0;
    cost = 0;
}


// move the claw to the prize position
int Claws::move_claw(vector<vector<int> > &M, coor pos, int nA, int nB){
    if(pos < origin || nA > MAX_MOVES || nB > MAX_MOVES){
        return MAX_VALUE; //infinite
    }
    if(M[pos.x][pos.y] != -1){
        return M[pos.x][pos.y];
    }   
    int cost = 0;
    if(pos == origin){
        M[0][0] = 0;
        //return 0;
    }
    else{
        long pressA = move_claw(M, pos - A, ++nA, nB) + TKN_A;
        long pressB = move_claw(M, pos - B, nA, ++nB) + TKN_B;
        M[pos.x][pos.y] = min(pressA, pressB);
        //return min(pressA, pressB);
    }
    return M[pos.x][pos.y];
}


//prints the configuration of teh machine
void Claws::show(){
    cout << "A: " << A.show() << endl;
    cout << "B: " << B.show() << endl;
    cout << "Prize: " << prize.show() << endl;
}


// get the prize position at minimum cost
void get_prize(coor A, coor B, coor prize){
    //memoization table
    vector<vector<int> > M(prize.x + 1, vector<int>(prize.y + 1, -1));
    Claws claws(A, B, prize);
    claws.show();
    int cost = claws.move_claw(M, prize, 0, 0);
    if(cost >= MAX_VALUE)
        cout << "No solution" << endl;
    else
        cout << "Minimum cost: " << cost << endl;
}


int main() {
    //load the input
    fstream input("testshort.txt");
    string line;
    char btn[20], name; 
    coor A, B, prize;
    while(getline(input, line)){
        sscanf(line.c_str(), "%s %c: X+%d, Y+%d", btn, &name, &A.x, &A.y);
        getline(input, line);
        sscanf(line.c_str(), "%s %c: X+%d, Y+%d", btn, &name, &B.x, &B.y);
        getline(input, line);
        //be careful: this include ":" in btn since read until space
        sscanf(line.c_str(), "%s X=%d, Y=%d", btn, &prize.x, &prize.y);
        getline(input, line);
        get_prize(A, B, prize);
    }
    input.close();
    return 0;
}