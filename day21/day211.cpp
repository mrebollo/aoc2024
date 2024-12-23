/*
    advent of code 21 (1)
    mark a code in a numeric keypad
    789
    456
    123
    *0A
     but marking over a directional pad
     *^A
     <v>     
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// button A is in 10th position
#define A 10
#define UP 0b00000001 //UP 1 x2 x3
#define DW 0b00000100 //DOWN 4
#define LF 0b00010000 //LEFT 16
#define RG 0b01000000 //RIGHT 64


// movements between two numbers in the keypad
//    789
//    456
//    123
//    *0A
const int distKB[11][11] = {
    {0, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, 3*UP+LF, 3*UP, 3*UP+RG, RG}, //from 0
    {LF+DW, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*UP, 2*UP+RG, 2*UP+2*RG, 2*RG+DW}, //from 1
    {DW, LF, 0, RG, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, DW+RG}, //from 2
    {DW+LF, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, DW}, //from 3
    {RG+2*DW, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*RG+2*DW}, //from 4
    {2*DW, DW+LF, DW, DW+RG, LF, 0, RG, UP+LF, UP, UP+RG, 2*DW+RG}, //from 5
    {2*DW+LF, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*DW}, //from 6
    {RG+3*DW, 2*DW, 2*DW+RG, 2*DW+2*RG, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, 2*RG+3*DW}, //from 7
    {3*DW, 2*DW+LF, 2*DW, 2*DW+RG, DW+LF, DW, DW+RG, LF, UP, RG, 3*DW+RG}, //from 8
    {3*DW+LF, 2*DW+2*LF, 2*DW+LF, 2*DW, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*DW}, //from 9
    {LF, UP+2*LF, UP+2*LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, 3*UP+2*LF, 3*UP+LF, 3*UP, 0}  //from A
};
// distance in directional pad
//  *^A
//  <v>  
const int distDIR[11][11] = {
    {0, DW, DW+LF, DW+RG, 0, 0, 0, 0, 0, 0, RG}, //from UP
    {UP, 0, LF, RG, 0, 0, 0, 0, 0, 0, UP+RG}, //from DW
    {RG+UP, RG, 0, 2*RG, 0, 0, 0, 0, 0, 0, RG+UP}, //from LFT
    {0}, {0}, {0}, {0}, {0}, {0}, {0},  //empty
    {LF, LF+DW, DW+2*LF, DW, 0, 0, 0, 0, 0, 0, 0}  //from RGT
};

int dir[4] = {UP, DW, LF, RG};

class Keypad{
    private:
        int kb[11][11];	
        Keypad *control;
        int dist(int from, int to);
    public:
        Keypad(const int kb[][11], Keypad *ctrl = nullptr);
        void print(){};
        void move(int dir) {};
        int type (string code);
};

Keypad::Keypad(const int kb[][11], Keypad *ctrl){
    for(int i = 0; i < 11; i++)
        for(int j = 0; j < 11; j++)
            this->kb[i][j] = kb[i][j];
    control = ctrl;
}


// convert bitwise keypreses to  number
int Keypad::dist(int from, int to){
    int press = 0;
    for(int i = 0; i < 4; i++)
        press += ((kb[from][to] & dir[i]) >> 2*i); // 2 bits per direction
    return press;
}


int Keypad::type(string code){
    int at = A, moves = 0, to;
    for(char c: code){
        // to finish goto A
        if (c == 'A') 
            return moves + dist(at, A);        
        to = c - '0';
        // movements
        moves += dist(at, to);
        at = to;
    }
}



int main() {
    string code;
    Keypad kb(distKB);
    int moves, total = 0;
    fstream inputf("test.txt");
    while(getline(inputf, code)){
        moves = kb.type(code);
        cout << "code: " << code << " moves: " << moves << endl;
        total += moves;
    }
    inputf.close();
    cout << "total moves: " << total << endl;
    return 0;
}