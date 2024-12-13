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
#include <queue>
#include <algorithm>

#define MAX_MOVES 100

using namespace std;

struct button{
    int x, y;
    string show(){
        return "(" + to_string(x) + "," + to_string(y) + ")";
    }
};



// get the prize position at minimum cost
void get_prize(button A, button B, button prize){
    cout << "A " << A.show() << endl;
    cout << "B " << B.show() << endl;
    cout << "prize " << prize.show() << endl;
}

int main() {
    //load the input
    fstream input("test.txt");
    string line;
    char btn[20], name; 
    button A, B, prize;
    while(getline(input, line)){
        sscanf(line.c_str(), "%s %c: X+%d, Y+%d", btn, &name, &A.x, &A.y);
        getline(input, line);
        sscanf(line.c_str(), "%s %c: X+%d, Y+%d", btn, &name, &B.x, &B.y);
        getline(input, line);
        sscanf(line.c_str(), "%s: X=%d, Y=%d", btn, &prize.x, &prize.y);
        getline(input, line);
        get_prize(A, B, prize);
    }
    input.close();
    return 0;
}