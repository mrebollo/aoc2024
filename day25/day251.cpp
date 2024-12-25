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

void add_lock(ifstream &file){
    vector<int> lock(5,0);
    string line;
    while(getline(file, line)){
        if(line.empty()) break;
        for(int i=0; i<5; i++) if(line[i] == '#') lock[i]++;
    }
    //divide by '#' to get the heigh
    locks.insert(lock);
}

void add_key(ifstream &file){
    vector<int> key(5,0);
    string line;
    while(getline(file, line)){
        if(all_sharp(line)) break;
        for(int i=0; i<5; i++) if(line[i] == '#') key[i]++;
    }
    //divide by '#' to get the heigh
    keys.insert(key);
}


void print(vector<int> scheme){
    for(int col: scheme) cout << col << ",";
    cout << endl;
}


void print(set<vector<int> > scheme){
    for(auto s : scheme)
        print(s);
}

void readSchemes(string filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        if(line.empty()) continue;
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

int main(){
    readSchemes("input.txt");
    return 0;
}

