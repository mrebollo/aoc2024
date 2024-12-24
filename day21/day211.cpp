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

     Modified to obtain all combinations and choose the one with less moves
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
    {LF+DW, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*UP, 2*UP+RG, 2*UP+2*RG, 2*RG+DW}, //from 1
    {DW, LF, 0, RG, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, DW+RG}, //from 2
    {DW+LF, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, DW}, //from 3
    {RG+2*DW, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*RG+2*DW}, //from 4
    {2*DW, DW+LF, DW, DW+RG, LF, 0, RG, UP+LF, UP, UP+RG, 2*DW+RG}, //from 5
    {2*DW+LF, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*DW}, //from 6
    {RG+3*DW, 2*DW, 2*DW+RG, 2*DW+2*RG, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, 2*RG+3*DW}, //from 7
    {3*DW, 2*DW+LF, 2*DW, 2*DW+RG, DW+LF, DW, DW+RG, LF, UP, RG, 3*DW+RG}, //from 8
    {3*DW+LF, 2*DW+2*LF, 2*DW+LF, 2*DW, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, 3*DW}, //from 9
    {LF, UP+2*LF, UP+2*LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, 3*UP+2*LF, 3*UP+LF, 3*UP, 0}  //from A
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

int dir[4] = {3, 12, 48, 192}; // 2^0, 2^2, 2^4, 2^6
string dirout = "^v<>      A";

class Keypad{
    private:
        int kb[11][11];        
        vector<string> typed;        
        int dist(int from, int to);
        int ch2pos(char c);
    public:
        Keypad(const int kb[][11]);
        void print(){};
        void move(int dir) {};
        vector<string> type (string code);
};


Keypad::Keypad(const int kb[][11]){
    for(int i = 0; i < 11; i++)
        for(int j = 0; j < 11; j++)
            this->kb[i][j] = kb[i][j];

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
        if(rep.first < count){
            rep.second = rep.first;
            rep.first = count;
            key.second = key.first;
            key.first = dirout[i];
        }
        else if(rep.second < count){
            rep.second = count;
            key.second = dirout[i];
        }
    }
    // process the keys to modify the output  
    // TODO: extract to a function
    if(rep.second == 0){
        for(string &s: typed){
            for(int r = 0; r < rep.first; r++)
                s += key.first;
            s += 'A';
        }
    }
    else{
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
    Keypad kb(distKB);
    Keypad dp(distDIR);
    Keypad ddp(distDIR);
    int total = 0;
    fstream inputf("input.txt");
    while(getline(inputf, code)){
        vector<string> aux, final;
        vector<string> output = kb.type(code);
        /*
        for(string &s: output)
            cout << code << ": " << s << " [" << s.size() << "]" << endl;
        */
       for(string &s: output){
            vector<string> output2 = dp.type(s);
            aux.insert(aux.end(), output2.begin(), output2.end());
        }
        /*
        for(string &s: aux)
            cout << code << ": " << s << " [" << s.size() << "]" << endl;
        */
       for(string &s: aux){
            vector<string> output3 = ddp.type(s);
            final.insert(final.end(), output3.begin(), output3.end());
        }
        /*
        for(string &s: final)
            cout << code << ": " << s << " [" << s.size() << "]" << endl;
        */
       // why is this not working?
       string smin = *min_element(final.begin(), final.end(), str_compare);
        int mlen = 9999, pos = 0;
        for(int i = 0; i < final.size(); i++){
            //cout << s << ": " << s.size() << endl;
            if (final[i].size() < mlen) {mlen = final[i].size(); pos = i;}
        }
        cout << code << ": " << smin << " [" << smin.size() << "]" << endl;
        cout << code << ": " << final[pos] << " [" << final[pos].size() << "]" << endl;
        code.pop_back();
        total += final[mlen].size() * stoi(code);
    }
    inputf.close();
    cout << "total moves: " << total << endl;
    return 0;
}


/*
029A: <A^A^^>AvvvA [12]
<A^A^^>AvvvA: v<<A^>>A<A>A<AAv>A^Av<AAA^>A [28]
v<<A^>>A<A>A<AAv>A^Av<AAA^>A: v<A<AA^>>A<Av>AA^Av<<A^>>AvA^Av<<A^>>AAv<A>A^A<A>Av<A<A^>>AAA<Av>A^A [68]
980A: ^^^A<AvvvA>A [12]
^^^A<AvvvA>A: <AAA>Av<<A^>>Av<AAA^>AvA^A [26]
<AAA>Av<<A^>>Av<AAA^>AvA^A: v<<A^>>AAAvA^Av<A<AA^>>A<Av>AA^Av<A<A^>>AAA<Av>A^Av<A^>A<A>A [60]
179A: ^<<A^^A>>AvvvA [14]
^<<A^^A>>AvvvA: <Av<AA^>>A<AA>AvAA^Av<AAA^>A [28]
<Av<AA^>>A<AA>AvAA^Av<AAA^>A: v<<A^>>Av<A<A^>>AA<Av>AA^Av<<A^>>AAvA^Av<A^>AA<A>Av<A<A^>>AAA<Av>A^A [68]
456A: ^^<<A>A>AvvA [12]
^^<<A>A>AvvA: <AAv<AA^>>AvA^AvA^Av<AA^>A [26]
<AAv<AA^>>AvA^AvA^Av<AA^>A: v<<A^>>AAv<A<A^>>AA<Av>AA^Av<A^>A<A>Av<A^>A<A>Av<A<A^>>AA<Av>A^A [64]
379A: ^A^^<<A>>AvvvA [14]
^A^^<<A>>AvvvA: <A>A<AAv<AA^>>AvAA^Av<AAA^>A [28]
¿can be done with less?
^ A ^^ <<  A   >> A vvv  A
<A>A<AAv<AA^>>AvAA^Av<AAA^>A
<A>A<AAv<AA^>>AvAA^Av<AAA^>A: v<<A^>>AvA^Av<<A^>>AAv<A<A^>>AA<Av>AA^Av<A^>AA<A>Av<A<A^>>AAA<Av>A^A [68]
can be done in 64
<   A   > A <   AA   v  < AA   ^ >>  A v  AA  ^ A v  < AAA   ^ > A : 
v<<A^>>AvA^Av<<A^>>AAv<A<A^>>AA<Av>AA^Av<A^>AA<A>Av<A<A^>>AAA<Av>A^A
total moves: 127900
*/