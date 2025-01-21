/*
    advent of code 24 (2)
    compute through logic gates
    results in wires with label beginning with 'z'

    swap 4 pairs of wires (8 wires) until in acts as a sum
    x + y = z

    the circuit implements a full adder (find error here?)
    https://www.geeksforgeeks.org/binary-adder-with-logic-gates/
    1st gate: half adder (sum and carry) 
        x00 XOR y00 = z00
        x00 AND y00 = c00 (jjj)
    2nd gate: full adder
        x01 XOR y01 = s01 (jbf)
        x01 AND y01 = bbb (cpp)
        s01 XOR c00 = z01 (jjj XOR jbf = z01)
        c00 AND s01 = ccc (jbf AND jjj = pss)
        bbb OR ccc = c01 (cpp OR pss = rtc)
    and so on... until z44 (last carry bit is z45)
        x44 XOR y44 = s44 ( - )
        x44 AND y44 = yyy (qhs)
        s44 XOR c43 = z44 ( - AND ggg = z44)
        c43 AND s44 = xxx (ggg AND wdq = vkm)
        yyy OR xxx = z45 (qhs OR vkm)

    find error forward or backwards? 
    recursive function to find the error?

    as circuit: (xi XOR yi) XOR c(i-1) = zi |  
                (xi AND yi) OR zi  = ci
*/
#include <cstdint> //for __int_64_t
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

enum gate {NOOP, AND, OR, XOR};
struct wire{
    string label;           // label of the wire
    gate op;                 // operation
    struct wire *in1, *in2; // inputs
    bool val;                // value (just in case)
    int fulladder;
    wire(): label(""), val(false), op(NOOP), in1(NULL), in2(NULL), fulladder(-1) {}
    wire(string l, bool v) : label(l), val(v), op(NOOP), fulladder(-1)  {}
    wire(string l, gate o, wire *in1, wire *in2) : label(l), op(o), in1(in1), in2(in2), fulladder(-1) {}
    wire(wire *w) : label(w->label), val(w->val), op(w->op), in1(w->in1), in2(w->in2), fulladder(w->fulladder) {}
};

struct adder_id{
    //true if w1 adder > w2 adder id (first in the queue)
    bool operator()(wire* w1, wire* w2)
    {return w1->fulladder < w2->fulladder;}
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
        vector<wire*> get_all_gates(int op);
        void print(){
            string op[4] = {"NOOP", "AND", "OR", "XOR"};
            for(wire w : table)
                if(w.label != "" && w.op != NOOP)
                    cout << w.fulladder << ": " << w.label << " <- " << w.in1->label << " " << op[w.op] << " " << w.in2->label << endl;
        }
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
    //cout << table[pos].label << endl;
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
    //cout << table[pos].label << " <- " << table[pos].in1->label << " " << table[pos].op << " " << table[pos].in2->label << endl;
    return &table[pos];
}

vector<wire*> Hash::get_all_gates(int op=-1){
    vector<wire*> res;
    for(int i = 0; i < table.size(); i++)
        if ((op == -1 && table[i].label != "") || table[i].op == op)
            res.push_back(&table[i]);
    return res;
}


class Circuit{
    private:
        Hash wires;
        //not working with out |= ((__int_64_t)1 << id); why?
        uint64_t in1, in2, out;
        vector<wire*> inx, iny, output;
        bool result(wire* w);
        void add_to_output(wire *wo){
            int id;
            sscanf((wo->label).c_str(), "z%d", &id);
            output[id] = wo;
            wo->fulladder = id;
        }
        void add_to_input(wire *wi){
            if(wi->label[0] == 'x') inx.push_back(wi);
            else iny.push_back(wi);
            wi->fulladder = atoi(wi->label.substr(1,2).c_str());
        }
        vector<string>  check_z();
        vector<string>  check_or();
        vector<string>  check_ands();
        vector<string>  check_xors();
    public:
        Circuit();
        void load_circuit(string filename);
        void solve();
        void correct();
        void print_output();      
        void print_sum();  
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
        wire *wi = wires.insert(w, val);
        add_to_input(wi);
    }
    //read gates
    string in1, in2, out, op, scum;
    while(getline(inputf, line)){
        stringstream ss(line);
        ss >> in1 >> op >> in2 >> scum >> out;
        wire *wo = wires.insert(out, op, in1, in2);
        if(out[0] == 'z'){
            add_to_output(wo);
        }
        //fulladder id from x__ or y__ rule
        if(wo->in1->label[0] == 'x' || wo->in1->label[0] == 'y'){
            int fad = atoi(wo->in1->label.substr(1,2).c_str());
            wo->fulladder = fad;
        }
        // pending case: intermediate with previos carry and sum
        // antecedents can be unreaded yet -> not to apply
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


// z has to be obtained as aaa XOR bbb
vector<string>  Circuit::check_z(){
    char label[4];
    vector<string> errors;
    //gates z00 and z45 are special cases (half adder and last carry)
    for(int i = 44; i > 0; i--){
        //get the output wire
        sprintf(label,"z%02d", i);
        wire *w = wires.find(label);
        if(w->op != XOR){
            cout << "z-error in wire " << w->label << endl;
            errors.push_back(w->label);
        }
    }
    return errors;
}

// an OR gate has to have AND gates as inputs
vector<string>  Circuit::check_or(){
    char label[4];
    vector<string> errors;
    for(wire *w : wires.get_all_gates(OR)){
        if(w->in1->op != AND){
            cout << "or-error in wire " << w->in1->label << endl;
            errors.push_back(w->in1->label);
        }
        if(w->in2->op != AND){
            cout << "or-error in wire " << w->in2->label << endl;
            errors.push_back(w->in2->label);
        }
    }
    return errors;
}

// XOR gate has to have x__ or y__ as inputs
// or the result th XOR-xy gate with an OR
vector<string> Circuit::check_xors(){
    vector<string> errors;
    for(wire *w : wires.get_all_gates(XOR)){
        // a non-z xor wire has to have x__ or y__ as inputs
        if(w->label[0] != 'z') continue; //z gates already checked
        if(w->label == "z00" || w->label == "z01") continue;  //z00 is a half adder and c00 is an AND gate
        if(w->in1->op == OR){
            if(w->in2->op != XOR || (w->in2->op == XOR && w->in2->in1->op != NOOP)){
                cout << "error in non-z XOR wire (XOR) " << w->in2->label << endl;
                errors.push_back(w->in2->label);
            }
        } 
        else if(w->in2->op == OR){
            if(w->in1->op != XOR || (w->in1->op == XOR && w->in1->in1->op != NOOP)){
                cout << "error in non-z XOR wire (XOR) " << w->in1->label << endl;
                errors.push_back(w->in1->label);
            }
        }
        //no OR
        else if(w->in1->op != XOR || (w->in1->op == XOR && w->in1->fulladder < 0)){
            cout << "error in non-z XOR wire (OR)" << w->in1->label << endl;
            errors.push_back(w->in1->label);
        }
        else if(w->in2->op != XOR || (w->in2->op == XOR && w->in2->fulladder < 0)){
            cout << "error in non-z XOR wire (OR)" << w->in2->label << endl;
            errors.push_back(w->in2->label);
        }
    }
    return errors;
}


// check correct AND chains: xi AND yi = auxi , auxi XOR --- = zi
vector<string>  Circuit::check_ands(){
    vector<string> errors;
    vector<wire*> andgates = wires.get_all_gates(AND);
    for(wire *w : andgates){
        // skip x__ and y__ wires for carry
        if(w->in1->label[0] == 'x' || w->in1->label[0] == 'y') continue;
        // check if the inputs are x__ XOR y__  and the other OR
         if(w->in1->op == OR){
            if(w->in2->op != XOR || (w->in2->op == XOR && w->in2->in1->op != NOOP)){
                cout << "error in AND wire " << w->in2->label << endl;
                errors.push_back(w->in2->label);
            }
        }
        else if(w->in2->op == OR){
            if(w->in1->op != XOR || (w->in1->op == XOR && w->in1->in1->op != NOOP)){
                cout << "error in AND wire " << w->in1->label << endl;
                errors.push_back(w->in1->label);
            }
        }
        //no OR
        else if(w->in1->op == XOR && w->in1->fulladder < 0){
            cout << "error in non-z AND wire (OR)" << w->in1->label << endl;
            errors.push_back(w->in1->label);
        }
        else if(w->in2->op == XOR && w->in2->fulladder < 0){
            cout << "error in non-z AND wire (OR)" << w->in2->label << endl;
            errors.push_back(w->in2->label);
        }
    }
    return errors;
}


void Circuit::correct(){
    set<string> errors;
    vector<string> err;
    err = check_z();
    errors.insert(err.begin(), err.end());
    err = check_or();
    errors.insert(err.begin(), err.end());
    //only one changes pending (2 gates)
    err = check_xors();
    errors.insert(err.begin(), err.end());
    //redundant
    err = check_ands();
    errors.insert(err.begin(), err.end());
 
    sort(err.begin(), err.end());
    for(string e: errors)
        cout << e << ",";
    cout << endl; 
}


int main() {
    Circuit c;
    c.load_circuit("input.txt");
    c.correct();
    return 0;
}


