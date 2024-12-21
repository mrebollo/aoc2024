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
#define Ad 4
// transpose the keypad to a 3x3 matrix
// top - down - left - right
const int d[4] = {-3, 3, -1, 1};
// distance from a number to A in the kb
const int toA[11] = {1, 3, 2, 1, 4, 3, 2, 5, 4, 3, 0};
// distance between two numbers in the kb
//    789
//    456
//    123
//    *0A
const int distKB[11][11] = {
    {0, 2, 1, 2, 3, 2, 3, 4, 3, 4, 1}, //from 0
    {2, 0, 1, 2, 1, 2, 3, 2, 3, 4, 2}, //from 1
    {1, 1, 0, 1, 2, 1, 2, 3, 2, 3, 2}, //from 2
    {2, 2, 1, 0, 3, 2, 1, 4, 3, 2, 3}, //from 3
    {3, 1, 2, 3, 0, 1, 2, 1, 2, 3, 4}, //from 4
    {2, 2, 1, 2, 1, 0, 1, 2, 1, 2, 3}, //from 5
    {3, 3, 2, 1, 2, 1, 0, 3, 2, 1, 2}, //from 6
    {4, 2, 3, 4, 1, 2, 3, 0, 1, 2, 5}, //from 7
    {3, 3, 2, 3, 2, 1, 2, 1, 0, 1, 4}, //from 8
    {4, 4, 3, 2, 3, 2, 1, 2, 1, 0, 3}, //from 9
    {1, 3, 2, 1, 4, 3, 2, 5, 4, 3, 0}  //from A
};
// distance in directional pad
//  *^A
//  <v>  
enum dir {UP, DW, LFT, RGT, Ad};
const int distDIR[5][5] = {
    {0, 1, 2, 2, 1}, //from UP
    {1, 0, 1, 1, 2}, //from DW
    {2, 1, 0, 2, 3}, //from LFT
    {2, 1, 2, 0, 1}  //from RGT
};

class Keypad{
    private:
        vector<int> kb;
    public:
        Keypad();
        void print(){};
        void move(int dir) {};
        int movements(string code);
};



Keypad::Keypad(){
    kb.resize(11,0);
}

int Keypad::movements(string code){
    int at = A, moves = 0, to;
    for(char c: code){
        // to finish goto A + press button A
        if (c == 'A') 
            return moves + distKB[at][A] + 1;        
        to = c - '0';
        // movements + press button
        moves += distKB[at][to] + 1;
        at = to;
    }
}



int main() {
    string code;
    Keypad kb;
    int moves, total = 0;
    fstream inputf("test.txt");
    while(getline(inputf, code)){
        moves = kb.movements(code);
        cout << "code: " << code << " moves: " << moves << endl;
        total += moves;
    }
    inputf.close();
    cout << "total moves: " << total << endl;
    return 0;
}