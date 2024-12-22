/*
advent of code day 19 (1)
identify vallid patterns formed from strips
-> valid words of a formal grammar
Recursive look for prefixes of the string that are in the dictionary until end or fail
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


// hash table with chaining for conflict resolution
class Hash{
    private:
        vector<vector<string> > table;
    public:
        Hash();
        void insert(string value){
            int key = value[0] - 'a';
            table[key].push_back(value);
        }
        vector<string>& get_all(char c){
            int key = c - 'a';
            return table[key];
        }
};

Hash::Hash(){
    table.resize(26);
}

Hash strips;

//return all prefixes of a string that are in the dictionary
vector<string>& obtain_prefixes(string s){
    return strips.get_all(s[0]);
}


//recursive function to check if a string is valid
bool is_valid(string s){
    if(s.size() == 0)
        return true;
    vector<string>& prefixes = obtain_prefixes(s);
    for (string prefix : prefixes){
        if(s.substr(0, prefix.size()) == prefix &&
           is_valid(s.substr(prefix.size())))
            return true;
    }
    return false;
}


//load dictionary from file
void load_dictionary(string line){
    stringstream ss(line);
    string word;
    while(getline(ss, word, ',')){
        // remove heading blanks
        if(word[0] == ' ')
            word = word.substr(1);
        strips.insert(word);
    }
}


int main() {
    ifstream file("input.txt");
    string line;
    //read strips from file
    getline(file, line);
    load_dictionary(line);
    //blank line
    getline(file, line);
    //read and analayze combinations
    int total_valid = 0;
    while(getline(file, line)){
        if (is_valid(line)){
            total_valid++;
            cout << line << " is valid [" << total_valid << "]" << endl;
        }
        else
            cout << line << " is invalid" << endl;
    }
    cout << "total: " << total_valid << endl;

    return 0;
}