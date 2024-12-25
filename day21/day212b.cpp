/*
    advent of code 21 (2)
    mark a code in a numeric keypad
    789
    456
    123
    *0A
     but marking over a directional pad
     *^A
     <v>     

    Same, but with 25 robots. Sum of the five moves lengths.
    Recover the moves calculation instead of creating the sequence, if possible 
    (since it involves 2^25 alternatives)
    Using distances between keys, and reversible movements
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// button A is in 10th position
#define A 10

// movements between two numbers in the keypad
//    789
//    456
//    123
//    *0A
int distKB[11][11] = {
    {0, 2, 1, 2, 3, 2, 3, 4, 3, 4, 1}, //from 0
    {2, 0, 1, 2, 1, 2, 3, 2, 3, 4, 3}, //from 1
    {1, 1, 0, 1, 2, 1, 2, 3, 2, 3, 2}, //from 2
    {2, 3, 1, 0, 3, 2, 1, 4, 3, 2, 1}, //from 3
    {3, 1, 2, 3, 0, 1, 2, 1, 2, 3, 4}, //from 4
    {3, 2, 1, 2, 1, 0, 1, 2, 1, 2, 3}, //from 5
    {3, 3, 2, 1, 3, 2, 0, 3, 2, 1, 2}, //from 6
    {4, 2, 3, 4, 1, 2, 3, 0, 1, 2, 5}, //from 7
    {3, 3, 2, 3, 2, 1, 2, 1, 0, 1, 4}, //from 8
    {4, 4, 3, 2, 3, 2, 1, 2, 1, 0, 3}, //from 9
    {1, 3, 2, 1, 4, 3, 2, 5, 4, 3, 0}  //from A
};
// distance in directional pad
//  *^A
//  <v>  
int distDIR[11][11] = {
    {0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 1}, //from UP
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2}, //from DW
    {2, 1, 0, 2, 0, 0, 0, 0, 0, 0, 3}, //from LFT
    {3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1},  //from RGT
    {0}, {0}, {0}, {0}, {0}, {0},
    {1, 2, 3, 1, 0, 0, 0, 0, 0, 0, 0}  //from A 
};


class Keypad{
    private:
        int kb[11][11];   
        Keypad *ctrl;
    public:
        Keypad(int kb[][11], Keypad *ctrl);
        void move(int dir);
        int type (string code);
    private:
        int dist(int from, int to);
};


Keypad::Keypad(int kb[][11], Keypad *ctrl = nullptr){
    this->ctrl = ctrl;
    for(int i = 0; i < 11; i++)
        for(int j = 0; j < 11; j++)
            this->kb[i][j] = kb[i][j];
}


// convert bitwise keypreses to  number
int Keypad::dist(int from, int to){
    return kb[from][to] + ((ctrl) ? ctrl->kb[from][to] + 1 : 0) + 1;
}


// keys to press in the directional pad to obtain the code
int Keypad::type(string code){
    int at = A, moves = 0, to;
    for(char c: code){
        to = (c == 'A') ? A : c - '0';
        moves += dist(at, to);
        at = to;
    }
    return moves;
}


int main() {
    string code;
    Keypad dp(&distDIR[0]);
    Keypad kb(&distKB[0], &dp);
    int total = 0;
    fstream inputf("test.txt");
    while(getline(inputf, code)){
        int moves =  kb.type(code);
        cout << "code: " << code << " - " << moves << endl;
        total += moves;
    }
    inputf.close();
    cout << "total moves: " << total << endl;
    return 0;
}

