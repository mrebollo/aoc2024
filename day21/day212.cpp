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
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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
    {RG+DW, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*UP, 2*UP+RG, 2*UP+2*RG, 2*RG+DW}, //from 1
    {DW, LF, 0, RG, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, DW+RG}, //from 2
    {DW+LF, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, DW}, //from 3
    {RG+2*DW, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*RG+2*DW}, //from 4
    {2*DW, DW+LF, DW, DW+RG, LF, 0, RG, UP+LF, UP, UP+RG, 2*DW+RG}, //from 5
    {2*DW+LF, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*DW}, //from 6
    {RG+3*DW, 2*DW, 2*DW+RG, 2*DW+2*RG, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, 2*RG+3*DW}, //from 7
    {3*DW, 2*DW+LF, 2*DW, 2*DW+RG, DW+LF, DW, DW+RG, LF, 0, RG, 3*DW+RG}, //from 8
    {3*DW+LF, 2*DW+2*LF, 2*DW+LF, 2*DW, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, 3*DW}, //from 9
    {LF, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, 3*UP+2*LF, 3*UP+LF, 3*UP, 0}  //from A
};
// distance in directional pad
//  *^A
//  <v>  
const int distDIR[11][11] = {
    {0, DW, DW+LF, DW+RG, 0, 0, 0, 0, 0, 0, RG}, //from UP
    {UP, 0, LF, RG, 0, 0, 0, 0, 0, 0, UP+RG}, //from DW
    {RG+UP, RG, 0, 2*RG, 0, 0, 0, 0, 0, 0, 2*RG+UP}, //from LFT
    {LF+UP, LF, 2*LF, 0, 0, 0, 0, 0, 0, 0, UP},  //from RGT
    {0}, {0}, {0}, {0}, {0}, {0},
    {LF, DW+LF, DW+2*LF, DW, 0, 0, 0, 0, 0, 0, 0}  //from A 
};

//reversible movements (not passing by black key [*])
// 1: reversible, 2: highest bits are the valid movements (LF/RG over UP/DW)
// TODO: change for a more intuitive meaning (ex: 0 no modif, 1, lower bits, 2 higher bits)
const int doubKB[11][11] = {
     {0,0,1,1,0,1,1,0,1,1,1}, 
     {2,0,1,1,1,1,1,1,1,1,2}, 
     {1,1,0,1,1,1,1,1,1,1,1}, 
     {1,1,1,0,1,1,1,1,1,1,1}, 
     {2,1,1,1,0,1,1,1,1,1,2}, 
     {1,1,1,1,1,0,1,1,1,1,1}, 
     {1,1,1,1,1,1,0,1,1,1,1}, 
     {2,1,1,1,1,1,1,0,1,1,2}, 
     {1,1,1,1,1,1,1,1,0,1,1}, 
     {1,1,1,1,1,1,1,1,1,0,1}, 
     {1,0,1,1,0,1,1,0,1,1,0}
};

const int doubDIR[11][11] = {
     {0,1,0,1,0,0,0,0,0,0,1}, 
     {1,0,1,1,0,0,0,0,0,0,1},
     {2,1,0,1,0,0,0,0,0,0,2}, 
     {1,1,1,0,0,0,0,0,0,0,1},
     {0}, {0}, {0}, {0}, {0}, {0},
     {1,1,0,1,0,0,0,0,0,0,0}
};


int dir[4] = {3, 12, 48, 192}; // 2^0, 2^2, 2^4, 2^6
string dirout = "^v<>      A";

class Keypad{
    private:
        int kb[11][11];   
        int db[11][11];     
        vector<string> typed;        
        int dist(int from, int to);
        int ch2pos(char c);
    public:
        Keypad(const int kb[][11], const int db[][11]);
        void print(){};
        void move(int dir) {};
        vector<string> type (string code);
};


Keypad::Keypad(const int kb[][11], const int db[][11]){
    for(int i = 0; i < 11; i++)
        for(int j = 0; j < 11; j++){
            this->kb[i][j] = kb[i][j];
            this->db[i][j] = db[i][j];
        }

}


// convert bitwise keypreses to  number
int Keypad::dist(int from, int to){
    int press = 0;
    pair<int, int> rep(0,0);
    pair<char, char> key(' ', ' ');
    bool once = false;
    for(int i = 0; i < 4; i++){
        int count = ((kb[from][to] & dir[i]) >> 2*i) ; // 2 bits per direction
        // saves key pressed in the first and second place
        if(count > 0)
            if(rep.first == 0){
                rep.first = count;
                key.first = dirout[i];
            }
            else {
                rep.second = count;
                key.second = dirout[i];
            }
    }
    // FAILURE: there are non-valid trajectories removed from distKB, but appear in the bitmask
    // ex: A to 1 (UP+2LF) valid, but not 2LF+UP since there is no key under 1
    // -> try to check and remove the invalids
    // process the keys to modify the output  
    // TODO: extract to a function
    if(rep.second == 0){
        for(string &s: typed){
            s.append(rep.first, key.first);
            s += 'A';
        }
    }
    else if (db[from][to] == 1){
        //all combinations are valid
        //make a copy of the existing
        vector<string> aux = typed;
        // modify existing with first key
        for(string &s: typed){
            s.append(rep.first, key.first);
            s.append(rep.second, key.second);
            s += 'A';
        }
        // add copy with second key
        for(string &s: aux){
            s.append(rep.second, key.second);
            s.append(rep.first, key.first);
            s += 'A';
            typed.push_back(s);
        }
    }
    else{
        // only one order is valid
        //TODO buscar la condición
        for(string &s: typed){
            if(db[from][to] == 2){
                s.append(rep.second, key.second);
                s.append(rep.first, key.first);
                
            }
            else{
                s.append(rep.first, key.first);
                s.append(rep.second, key.second);
            }
            s += 'A';
        }
    }
    return 0;
}


// convert char to position in the keyboard
int Keypad::ch2pos(char c){
    //directional pad hasn't "5" key
    if(kb[5][0])
        return (c == 'A') ? A : c - '0';
    else
        return dirout.find(c);
}

// keys to press in the directional pad to obtain the code
vector<string> Keypad::type(string code){
    int at = A, moves = 0, to;
    typed.clear();
    typed.push_back("");
    for(char c: code){
        if(c == ' ') continue;
        to = ch2pos(c);
        moves += dist(at, to);
        at = to;
    }
    return typed;
}

bool str_compare(string &a, string &b) {
    return a.size() < b.size(); 
}

int main() {
    string code;
    Keypad kb(distKB, doubKB);
    Keypad dp(distDIR, doubDIR);
    Keypad ddp(distDIR, doubDIR);
    int total = 0;
    fstream inputf("input.txt");
    while(getline(inputf, code)){
        vector<string> aux, final;
        vector<string> output = kb.type(code);
        for(string &s: output){
            //cout << code << ": " << s << endl;
            vector<string> output2 = dp.type(s);
            aux.insert(aux.end(), output2.begin(), output2.end());
        }
        for(string &s2: aux){
            //cout << code << ": " << s2 << endl;
            vector<string>output3 = ddp.type(s2);
            final.insert(final.end(), output3.begin(), output3.end());
        }
        string smin = *min_element(final.begin(), final.end(), str_compare);
        // does not work with lambda function insted of str_compare(??)
        // remove the final 'A' to get the code
        code.pop_back();
        total += smin.size();
    }
    inputf.close();
    cout << "total moves: " << total << endl;
    return 0;
}

