/*
advent of code day 13 (2)
claws machine
move the claw to the target position to win the prize
two butons A, B with different speeds and costs (A 3tk, B 1tk)
but sum 10000000000000 to the prize's coordinates
-> solve analitycally 

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "linalg.h"


#define EXCESS 10000000000000
#define MAX_VALUE 99999
#define MAX_MOVES 100
#define TKN_A 3
#define TKN_B 1

using namespace std;

struct coor{
    long long x, y;
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
    coor operator +(const coor &b){
        return {x + b.x, y + b.y};
    }
};
coor origin = {0, 0};


class Claws{
private:
    coor A, B, prize;
    long long  cost;
public:
    Claws(coor A, coor B, coor prize);
    void show();
};


Claws::Claws(coor A, coor B, coor prize){
    this->A = A;
    this->B = B;
    this->prize = prize;
}


//prints the configuration of teh machine
void Claws::show(){
    cout << "A: " << A.show() << endl;
    cout << "B: " << B.show() << endl;
    cout << "Prize: " << prize.show() << endl;
}


// get the prize position at minimum cost
long long get_prize(coor A, coor B, coor prize){
    //define equation system for simplex problem
    // x: A moves, y: B moves
    // min cost = 3x + y
    // sujeto a las restricciones
    // x * A.x + y * B.x = prize.x
    // x * A.y + y * B.y = prize.y
    // x, y >= 0
    // use the graphical method: intersection of the lines
    // x = (prize.x - y * B.x) / A.x = (prize.y - y * B.y) / A.y 
    // A.y(prize.x - y * B.x) = A.x(prize.y - y * B.y)
    // A.y * prize.x - A.y * y * B.x = A.x * prize.y - A.x * y * B.y
    // A.y * y * B.x - A.x * y * B.y = A.y * prize.x - A.x * prize.y
    lldiv_t ydiv = lldiv((A.y * prize.x - A.x * prize.y), (A.y * B.x - A.x * B.y));
    // substitute in the first equation
    lldiv_t x1div = lldiv((prize.x - ydiv.quot * B.x), A.x);
    lldiv_t x2div = lldiv((prize.y - ydiv.quot * B.y), A.y);
    //cout << "A: " << A.show() << " B: " << B.show() << " Prize: " << prize.show() << " Moves: " << x << " " << y << endl;
    cout << "A: " << A.show() << " B: " << B.show() << " Prize: " << prize.show() << " Moves: " << x1div.quot << "|" << x2div.quot << " "<< ydiv.quot << endl;
    if( ydiv.rem >0 || x1div.rem > 0 || 
        x1div.quot < 0 || ydiv.quot < 0 ){
        //|| x1div.quot > MAX_MOVES || ydiv.quot > MAX_MOVES){
        cout << "\t-> No solution" << endl;
        return 0;
    }
    else 
        cout << "\t-> Cost: " << x1div.quot * TKN_A + ydiv.quot * TKN_B << endl;
        return x1div.quot * TKN_A + ydiv.quot * TKN_B;
}


int main() {
    //load the input
    fstream input("input.txt");
    string line;
    char btn[20], name; 
    coor A, B, prize;
    long long tokens = 0;
    while(getline(input, line)){
        sscanf(line.c_str(), "%s %c: X+%lld, Y+%lld", btn, &name, &A.x, &A.y);
        getline(input, line);
        sscanf(line.c_str(), "%s %c: X+%lld, Y+%lld", btn, &name, &B.x, &B.y);
        getline(input, line);
        //be careful: this include ":" in btn since read until space
        sscanf(line.c_str(), "%s X=%lld, Y=%lld", btn, &prize.x, &prize.y);
        getline(input, line);
        prize.x += EXCESS;
        prize.y += EXCESS;
        tokens += get_prize(A, B, prize);
    }
    input.close();
    cout << "Tokens: " << tokens << endl;
    return 0;
}