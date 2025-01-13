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
        x44 XOR y44 = zzz ( - )
        x44 AND y44 = yyy (qhs)
        zzz XOR c43 = z44 ( - AND ggg = z44)
        c43 AND zzz = xxx (ggg AND wdq = vkm)
        yyy OR xxx = z45 (qhs OR vkm)

    find error forward or backwards? 
    recursive function to find the error?

    as circuit: (xi XOR yi) XOR c(i-1) = zi |  
                (xi AND yi) OR zi  = ci
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
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
        vector<wire> get_all_gates(int op);
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

vector<wire> Hash::get_all_gates(int op=-1){
    vector<wire> res;
    for(wire w : table)
        if ((op == -1 && w.label != "") || w.op == op)
            res.push_back(w);
    return res;
}


class Circuit{
    private:
        Hash wires;
        //not working with out |= ((__int_64_t)1 << id); why?
        __int64_t in1, in2, out;
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
        void propagate_adder_id();
        vector<string>  check_sums();
        vector<string>  check_carries();
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
            // fulladder id from z__ rule
            if(wo->in1->op == XOR)
                wo->in1->fulladder = atoi(wo->label.substr(1,2).c_str());
            else if(wo->in1->op == OR)
                wo->in1->fulladder = atoi(wo->label.substr(1,2).c_str())-1;
            if(wo->in2->op == XOR)
                wo->in2->fulladder = atoi(wo->label.substr(1,2).c_str());
            else if(wo->in2->op == OR)
                wo->in2->fulladder = atoi(wo->label.substr(1,2).c_str())-1;
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
    propagate_adder_id();
}


//propagates fulladders id to complete the wire's information
void Circuit::propagate_adder_id(){
    for(wire &w : wires.get_all_gates()){
        if(w.label == "") continue;
        if(w.fulladder == -1){
            // wire belongs to the same fulladder from its operators,
            // except in the carry case (from previous one)
            cout << w.in1->label << " " <<  w.op << " " << w.in2->label << " -> " << w.label << " pending" << endl;

        }
    }
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

void Circuit::print_sum(){
    cout << '0';
    for(int i = 46; i >= 0; i--)
        if(inx[i] != NULL)
            cout << (inx[i]->val ? '1' : '0');
    cout << endl << '0';
    for(int i = 46; i >= 0; i--)
        if(iny[i] != NULL)
            cout << (iny[i]->val ? '1' : '0');
    cout << endl;
    for(int i = 46; i >= 0; i--)
        if(output[i] != NULL)
            cout << (output[i]->val ? '1' : '0');

    cout << endl << "bitmasked: " << (__int64_t)out << endl;
}

bool is_sum_gate(int i, wire *w){
    //get the ops
    if(w->op != XOR) return false;
    //get the id of the inputs
    //(remove the first character of the label)
    int id = atoi(w->in1->label.substr(1,2).c_str());
    return (id == i);
}

bool is_overflow_gate(wire *w){
    //if the operator is an AND gate...
    if(w->op != AND) return false;
    //...operating over x__ or y__
    return (w->in1->label[0] == 'x' || w->in1->label[0] == 'y');
}

bool is_and_gate(wire *w){
    //if the operator is an XOR gate...
    if(w->op != XOR) return false;
    //...operating over x__ or y__
    return (w->in1->label[0] == 'x' || w->in1->label[0] == 'y');
}


bool is_xor_gate(wire *w){
    //if the operator is an AND gate...
    if(w->op != AND) return false;
    //...operating over x__ or y__
    return (w->in1->label[0] == 'x' || w->in1->label[0] == 'y');
}

/* bool Circuit::is_full_adder(string x, string y, string z, string c0, string c1){
    wire *wx = wires.find(x);
    wire *wy = wires.find(y);
    wire *wz = wires.find(z);
    wire *wc0 = wires.find(c0);
    wire *wc1 = wires.find(c1);
} */

// check correct sums chains: xi XOR yi = auxi , auxi XOR --- = zi
vector<string>  Circuit::check_sums(){
    char label[4];
    vector<string> errors;
    //gates z00 and z45 are special cases (half adder and last carry)
    //corect in my input
    for(int i = 44; i > 0; i--){
        //get the output wire
        sprintf(label,"z%02d", i);
        wire *w = wires.find(label);
        //correct if in1 or in2 is x(i-1) XOR y(i-1)
        if(!is_sum_gate(i, w->in1) && !is_sum_gate(i, w->in2)){
            cout << "error in wire " << w->label << endl;
            errors.push_back(w->label);
        }
    }
    return errors;
}


// check correct carry chains: xi AND yi = auxi , auxi OR --- = ci
vector<string>  Circuit::check_carries(){
    vector<string> errors;
    vector<wire> orgates = wires.get_all_gates(OR);
    for(wire &w : orgates){
        if(!is_overflow_gate(w.in1) && !is_overflow_gate(w.in2)){
            cout << "error in carry wire " << w.label << endl;
            errors.push_back(w.label);
        }
        else{
            //both have to be AND gates. overflow is, check the other
            if(w.in1->op != AND){
                cout << "error in carry wire " << w.in1->label << endl;
                errors.push_back(w.in1->label);
            }
            else if(w.in2->op != AND){
                cout << "error in carry wire " << w.in2->label << endl;
                errors.push_back(w.in2->label);
            }
        }
    }
    return errors;
} 


// check correct AND chains: xi AND yi = auxi , auxi XOR --- = zi
vector<string>  Circuit::check_ands(){
    vector<string> errors;
    vector<wire> andgates = wires.get_all_gates(AND);
    for(wire &w : andgates){
        // skip x__ and y__ wires for carry
        if(w.in1->label[0] == 'x' || w.in1->label[0] == 'y') continue;
        // check if the inputs are x__ XOR y__ 
        if(!is_and_gate(w.in1) && !is_and_gate(w.in2)){
            cout << "error in AND wire " << w.label << endl;
            errors.push_back(w.label);
        }
    }
    return errors;
}

// check correct XOR chains
vector<string> Circuit::check_xors(){
    vector<string> errors;
    vector<wire> xorgates = wires.get_all_gates(XOR);
    for(wire &w : xorgates){
        // skip x__ and y__ wires for carry
         if(w.label[0] != 'z' && w.fulladder == -1){
            cout << "error in XOR wire " << w.label << endl;
            errors.push_back(w.label);
         }
    }
    return errors;
}


void Circuit::correct(){
    set<string> errors;
    vector<string> err;
    err = check_sums();
    errors.insert(err.begin(), err.end());
    err = check_carries();
    errors.insert(err.begin(), err.end());
    err = check_ands();
    errors.insert(err.begin(), err.end());
    err = check_xors();
    errors.insert(err.begin(), err.end());
    sort(err.begin(), err.end());
    for(string e: errors)
        cout << e << ",";
    cout << endl;
}

int main() {
    Circuit c;
    c.load_circuit("input.txt");
    c.solve();
    c.print_output();
    c.print_sum();
    c.correct();
    cout << c.get_output() << endl;
    return 0;
}


