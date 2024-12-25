/*
    advent of code 25 (1)

    Detect matching pairs of keys and locks (columns heigh)

    Reduce combinations: sort keys and locks, then compare
    - read the schemes
    - traduce to array of heighs
    - sort the arrays
    - compare the arrays
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

set<vector<int> > keys;
set<vector<int> > locks;
//define a persosnalized order
/*
bool operator< (vector<int> &v1, vector<int> &v2){
    return v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2] && v1[3] < v2[3] && v1[4] < v2[4];   
}
*/

inline bool all_sharp(string s) {return s == "#####";}
inline bool is_lock(string s) {return all_sharp(s);}

// overload << to print a vector
ostream& operator<<(ostream &os, vector<int> v){
    for(int i: v) os << i << " ";
    return os;
}

void print(set<vector<int> > scheme){
    for(auto s : scheme)
        cout << s << endl;
}


void add_lock(ifstream &file){
    vector<int> lock(5,0);
    string line;
    while(getline(file, line)){
        if(line.empty()) break;
        for(int i=0; i<5; i++) if(line[i] == '#') lock[i]++;
    }
    locks.insert(lock);
}

void add_key(ifstream &file){
    vector<int> key(5,0);
    string line;
    while(getline(file, line)){
        if(line.empty()) break;
        for(int i=0; i<5; i++) if(line[i] == '#') key[i]++;
    }
    //remove last line since is a marker for end of key
    for(int i=0; i<5; i++) key[i]--;
    keys.insert(key);
}


void readSchemes(string filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        if(is_lock(line))
            add_lock(file);
        else
            add_key(file); 
    }
    file.close();
    cout << "Locks: " << locks.size() << endl;
    print(locks);
    cout << "Keys: " << keys.size() << endl;
    print(keys);
}


bool matches(vector<int> lock, vector<int> key){
    for(int i=0; i<5; i++)
        if(key[i] + lock[i] > 5) return false;
    return true;
}


int match(){
    int count = 0;
    for(auto lock: locks){
        for(auto key: keys){
            if(matches(lock, key)){
                cout << "lock " << lock << " matches key " << key << endl;  
                count++;
            }
        }
    }
    return count;
}

int main(){
    readSchemes("input.txt");
    int matches = match();
    cout << "Matches: " << matches << endl;
    return 0;
}

