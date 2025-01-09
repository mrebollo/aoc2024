/*
advent of code day 17 (1)
implements a 3-bit computer. Calculates output
operands: literal (its value) or combo (codified value)
- combo op  0 to 3 literal values (in binary) -> 0-7 
- combo op  4: register A value
- combo op  5: register B value
- combo op  6: register C value
- combo op  7: unused
instruction (codified in 3 bits)
    - op 0 adv: A = A / combo op^2
    - op 1 bxl: B = B XOR literal op
    - op 2 bst: B = combo op % 8
    - op 3 jnz: op pointr = literal op if A != 0
    - op 4 bxc: B = B XOR C
    - op 5 out: out <- combo op % 8
    - op 6 bdv: B = A / combo op^2
    - op 7 cdv: C = A / combo op^2
program: sequence [opcode, operand]+
ptr: pointer to the current instruction
registers: A, B, C
output: the output of the program
ptr += 2 after each instruction (except jnz)

Check https://www.reddit.com/r/adventofcode/comments/1hgig79/2024_day_17_part_2_i_need_the_hit_me_over_the/
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define _DEBUG 0


class Computer{
    private:
        enum op {adv, bxl, bst, jnz, bxc, out, bdv, cdv};
        string opname[8] = {"adv", "bxl", "bst", "jnz", "bxc", "out", "bdv", "cdv"};
        enum regid {A=4, B, C};
    public:
        Computer(string filename);
        void run();
        void print_state();
        void print_program();
    private:
        vector<int> program;
        vector<int> registers;
        vector<int> output;
        int ptr;
        int get_operand(int op);
};

Computer::Computer(string filename){
    fstream inputf(filename);
    string line;
    registers.resize(7, 0);
    ptr = 0;
    // read registers
    getline(inputf, line);
    sscanf(line.c_str(), "Register A: %ld", &registers[A]);
    getline(inputf, line);
    sscanf(line.c_str(), "Register B: %ld", &registers[B]);
    getline(inputf, line);
    sscanf(line.c_str(), "Register C: %ld", &registers[C]);
    // blank line
    getline(inputf, line);
    // load program
    getline(inputf, line);
    stringstream ss(line);
    //remove the word "program:" at the beginning
    string token;
    ss >> token;
    while(getline(ss, token, ','))
        program.push_back(stoi(token));
    inputf.close();
}


int Computer::get_operand(int op){
    return (op < 4) ? op : registers[op];
}


void Computer::run(){
    int op, combo, literal;
    while(ptr < program.size()){
        op = program[ptr];
        literal = program[ptr+1];
        combo = get_operand(literal);
        switch(op){
            case adv:
                registers[A] /= pow(2,combo);
                break;
            case bxl:
                registers[B] ^= literal;
                break;
            case bst:
                registers[B] = combo % 8;
                break;
            case jnz:
                if(registers[A] != 0){
                    ptr = literal;
                    continue; // skip ptr += 2
                }
                break;
            case bxc:
                registers[B] ^= registers[C];
                break;
            case out:
                output.push_back(combo % 8);
                break;
            case bdv:
                registers[B] = registers[A] / pow(2,combo);
                break;
            case cdv:
                registers[C] = registers[A] / pow(2,combo);
                break;
        }
#ifdef _DEBUG
        cout << "op: " << opname[op] << " literal: " << literal << " combo: " << combo << endl;
        print_state();
#endif
        ptr += 2;
    }
}


void Computer::print_state(){
    cout << "Register A: " << oct << registers[A] << endl;
    cout << "Register B: " << oct << registers[B] << endl;
    cout << "Register C: " << oct << registers[C] << endl;
    cout << "Output: ";
    for(int out : output)
        cout << out << ",";
    cout << endl;
}

void Computer::print_program(){
    for(int i = 0; i < program.size(); i+=2)
        cout << "[" << program[i] << ", " << program[i+1] << "] ";
    cout << endl;
}



int main(){
    Computer cmp("debug.txt");
    cmp.print_state();
    cmp.print_program();
    cmp.run();
    cmp.print_state();
    return 0;
}