/*
    advent of code 24 (2)
    compute through logic gates
    results in wires with label beginning with 'z'

    swap 4 pairs of wires (8 wires) until in acts as a sum
    x + y = z
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

enum gate {NOOP, AND, OR, XOR};
struct wire{
    string label;           // label of the wire
    gate op;                 // operation
    struct wire *in1, *in2; // inputs
    bool val;                // value (just in case)
    wire(): label(""), val(false), op(NOOP), in1(NULL), in2(NULL) {}
    wire(string l, bool v) : label(l), val(v), op(NOOP) {}
    wire(string l, gate o, wire *in1, wire *in2) : label(l), op(o), in1(in1), in2(in2){}
};

#define SIZE 719 // prime number for hash table

class Hash{
    private:
        vector<wire> table;
        int nitems;
        inline int hash(string label){ return (label[0] + label[1] + label[2]) % table.size(); }
    public:
        Hash(){
            table.resize(SIZE);
            nitems = 0;
        }
        wire* insert(string label, int val);
        wire* insert(string label, string op, string in1, string in2);
        wire* find(string label);
        gate strop(string op);
};

// return gate type
gate Hash::strop(string op){
    if(op == "AND") return AND;
    if(op == "OR") return OR;
    if(op == "XOR") return XOR;
    return NOOP;
}

// find wire in the table
wire* Hash::find(string label){
    int pos = hash(label);
    while(table[pos].label != "" && table[pos].label != label)
        pos = (pos + 1) % table.size();
    if(table[pos].label == "")
        table[pos] = wire(label, false);
    return &table[pos];
}


// insert final node
wire* Hash::insert(string label, int val){
    int pos = hash(label);
    //insert with linear probing
    while(table[pos].label != "")
        pos = (pos + 1) % table.size();
    table[pos] = wire(label, (bool)val);
    cout << table[pos].label << endl;
    nitems++;
    return &table[pos];
}

// insert gate
wire* Hash::insert(string label, string op, string in1, string in2){
    wire* w1 = find(in1);
    wire* w2 = find(in2);
    int pos = hash(label);
    //insert with linear probing
    while(table[pos].label != "" && table[pos].label != label)
        pos = (pos + 1) % table.size();
    // i can't do find(in1) here because it will create a new wire if it doesn't exist
    // and it could match with pos and corrupt the data
    table[pos] = wire(label, strop(op), w1, w2);
    cout << table[pos].label << " <- " << table[pos].in1->label << " " << table[pos].op << " " << table[pos].in2->label << endl;
    return &table[pos];
}


class Circuit{
    private:
        Hash wires;
        //not working with out |= ((__int_64_t)1 << id); why?
        __int64_t in1, in2, out;
        vector<wire*> output;
        bool result(wire* w);
        void add_to_output(wire *wo){
            int id;
            sscanf((wo->label).c_str(), "z%d", &id);
            output[id] = wo;
        }

    public:
        Circuit();
        void load_circuit(string filename);
        void solve();
        void print_output();        
        long long get_output(){
            long long res = 0, factor = 1;
            for(int i = 0; i < 46 && output[i] != NULL; i++){
                res += output[i]->val * factor;
                factor *= 2;
            }
            return res;
        }
};

Circuit::Circuit(){
    wires = Hash();
    output.resize(64);
    in1 = in2 = out = 0;
}


void Circuit::load_circuit(string filename){
    fstream inputf(filename);
    string line, w;
    int val;
    //read input
    while(getline(inputf, line)){
        if(line.empty()) break;
        stringstream ss(line);
        getline(ss, w, ':');
        ss >> val;
        wires.insert(w, val);
    }
    //read gates
    string in1, in2, out, op, scum;
    while(getline(inputf, line)){
        stringstream ss(line);
        ss >> in1 >> op >> in2 >> scum >> out;
        wire* wo = wires.insert(out, op, in1, in2);
        if(out[0] == 'z') add_to_output(wo);
    }
    inputf.close();
}


// calculate the value of a wire
// analyzing the  in postorder
bool Circuit::result(wire* w){
    if(w->op == NOOP) return w->val;
    if(w->op == AND) return result(w->in1) && result(w->in2);
    if(w->op == OR) return result(w->in1) || result(w->in2);
    if(w->op == XOR) return result(w->in1) ^ result(w->in2);
    return false;
}

// solve the circuit from each 'z' wire
void Circuit::solve(){
    for(int i = 0; i < 46 && output[i] != NULL; i++)
        output[i]->val = result(output[i]);
}


void Circuit::print_output(){
    for(int i = 46; i >= 0; i--)
        if(output[i] != NULL)
            cout << (output[i]->val ? '1' : '0');
    cout << endl;
    cout << "bitmasked: " << (__int64_t)out << endl;
}


int main() {
    Circuit c;
    c.load_circuit("input.txt");
    c.solve();
    c.print_output();
    cout << c.get_output() << endl;
    return 0;
}


