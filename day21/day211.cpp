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
    Use bitmasking to represent the buttons pressed
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// button A is in 10th position
#define A 10
#define Ad 4
// octal mask for the directional pad (max 3 pulses-> 2 bits)
#define UP 0b00000001 //1, 2, 3
#define DW 0b00000100 //4, 8, 12
#define LF 0b00010000 //16, 32, 48
#define RG 0b01000000 //64, 128, 192
char dirch[] = {'^', 'v', '<', '>', ' ', ' ', ' ', ' ', ' ', 'A'};

// distance between two numbers in the kb
//    789
//    456
//    123
//    *0A
const int distKB[11][11] = {
    {0, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, 3*UP+LF, 3*UP, 3*UP+RG, RG}, //from 0
    {DW+RG, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*UP, 2*UP+RG, 2*UP+2*RG, DW+2*RG}, //from 1
    {DW, LF, 0, RG, UP+LF, UP, UP+RG, 2*UP+LF, 2*UP, 2*UP+RG, DW+RG}, //from 2
    {DW+LF, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, DW}, //from 3
    {2*DW+RG, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, UP, UP+RG, UP+2*RG, 2*DW+2*RG}, //from 4
    {2*DW, DW+LF, DW, DW+RG, LF, 0, RG, UP+LF, UP, UP+RG, 2*DW+RG}, //from 5
    {2*DW+LF, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, UP+2*LF, UP+LF, UP, 2*DW}, //from 6
    {3*DW+RG, 2*DW, 2*DW+RG, 2*DW+2*RG, DW, DW+RG, DW+2*RG, 0, RG, 2*RG, 3*DW+2*RG}, //from 7
    {3*DW, 2*DW+LF, 2*DW, 2*DW+RG, DW+LF, DW, DW+RG, LF, 0, RG, 3*DW+RG}, //from 8
    {3*DW+LF, 2*DW+2*LF, 2*DW+LF, 2*DW, DW+2*LF, DW+LF, DW, 2*LF, LF, 0, 3*DW}, //from 9
    {LF, UP+2*LF, UP+LF, UP, 2*UP+2*LF, 2*UP+LF, 2*UP, 3*UP+2*LF, 3*UP+LF, 3*UP, 0} //from A
};
// distance in directional pad
//  *^A
//  <v>  
const int distDIR[11][11] = {
    {0, DW, DW+LF, DW+RG, 0, 0, 0, 0, 0, 0, RG}, //from UP
    {UP, 0, LF, RG, 0, 0, 0, 0, 0, 0, UP+RG}, //from DW
    {UP+RG, RG, 0, 2*RG, 0, 0, 0, 0, 0, 0, UP+2*RG}, //from LFT
    {UP+LF, LF, 2*LF, 0, 0, 0, 0, 0, 0, 0, UP},  //from RGT
    {0}, {0}, {0}, {0}, {0}, {0}, //empty
    {LF, DW+LF, DW+2*LF, DW, 0, 0, 0, 0, 0, 0, 0} //from A
};

string output;

class Keypad{
    private:
        int kb[11][11];  //distances between keys 
        size_t size;     //number of keys (size of the distances matrix)   
        Keypad *control; //who controls the keypad remotelly
        int at;          //current position
        int dist(int from, int to);
        int directions(int from, int to, Keypad *controlled);
        int nmoves(int from, int to);
    public:
        Keypad(const int kb[][11], Keypad *ctrl);
        int type(string code);
};



Keypad::Keypad(const int kb[][11], Keypad *ctrl = nullptr){
    size = 11;
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            this->kb[i][j] = kb[i][j];
    at = A;
    this->control = ctrl;
}

// obtain distance from btw operations
int Keypad::dist(int from, int to){
    int bitw = kb[from][to];
    int moves = 0;
    for(int i = 0; i < 4; i++){
        int m = (bitw >> i * 2) & 3;
        moves += m;
        for(int j = 0; j < m; j++)
            output.push_back(dirch[i]);
    }
    //moves += (bitw >> 0) & 3; //UPs 1
    //moves += (bitw >> 2) & 3; //DWs 4
    //moves += (bitw >> 4) & 3; //LFs 16
    //moves += (bitw >> 6) & 3; //RGs 64
    output += 'A';
    return moves;
}

// obtain distances in the directional pad
int Keypad::directions(int from, int to, Keypad *controlled){
    //obtain the movement for the desired code in the controlled pad
    int bitw = controlled->kb[from][to];
    int rep, moves = 0, redirect = 0;
    // for each direction
    for(int i = 0; i < 4; i++){ 
        //obtain the number of repetitions
        rep = (bitw >> i * 2) & 3;
        //obtain the corresponding movements in the directional pad
        if(rep > 0){
            redirect = nmoves(at, i);
            moves += redirect + rep - 1;
            at = i;
        }
        //accumulate the movements
        //repetitios just implies press the button again in the same direction
        // -1 because the first press is included in nmoves()
    }
    //back to A to press it when the sequence to press the number is finished
    //(can be obtained analyticaly beause it is a cycle)
    moves += nmoves(at, A);
    at = A;
    // return moves (includes +1 for pressing A)
    return moves;
}


// calculate needed movements to type a code
 int Keypad::nmoves(int from, int to) {
    // controls the numeric keyboard -> direct distance plkus 1 to press
    if(control == nullptr)
        return dist(from, to) + 1;
    // movements in the directional pad
    else
        return control->directions(from, to, this);
}

int Keypad::type(string code){
    int total_moves = 0, to;
    output.clear();
    cout << code << endl;
    for(char c: code){
        // convert the character to a number  
        to = (c == 'A') ? A : c - '0';
        // movements including pressing the button (+1)
        total_moves += nmoves(at, to);
        // update the current position
        at = to;
    }
    cout << output << endl;
    return total_moves;
}


//remove tha lasta character and convert the rest to an integer
int tonumber(string s){
    s.pop_back();
    return stoi(s);
}


int main() {
    string code;
    Keypad cold(distDIR);
    Keypad radiation(distDIR, &cold);
    Keypad numeric(distKB, &radiation);
    // use numeric keypad directly
    //Keypad numeric(distKB);
    
    fstream inputf("test.txt");
    int moves, complexity = 0;
    while(getline(inputf, code)){
        moves = numeric.type(code);
        cout << "code: " << code << " moves: " << moves << endl;
        complexity += tonumber(code) * moves;
    }
    inputf.close();
    cout << "total complexity: " << complexity << endl;
    return 0;
}

/* test output
029A: <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A 68
980A: <v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A 60
179A: <v<A>>^A<vA<A>>^AAvAA<^A>A<v<A>>^AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A 68
456A: <v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A 64
379A: <v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A 64


*/