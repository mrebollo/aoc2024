
/*
advent of code day 19 (2)
identify vallid patterns formed from strips
-> valid words of a formal grammar
Recursive look for prefixes of the string that are in the dictionary until end or fail
Count all possible combinations
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;


// hash table with chaining for conflict resolution
class Hash{
    private:
        vector<vector<string> > table;
    public:
        Hash(){ table.resize(26);}
        void insert(string value){
            int key = value[0] - 'a';
            table[key].push_back(value);
        }
        vector<string>& get_all(char c){
            int key = c - 'a';
            return table[key];
        }
        void show_dict(){
            for (int i = 0; i < 26; i++){
                cout << (char)('a' + i) << ": ";
                for (string s: table[i])
                    cout << s << " ";
                cout << endl;
            }
        }
};



Hash strips;

//return all prefixes of a string that are in the dictionary
vector<string>& obtain_prefixes(string s){
    return strips.get_all(s[0]);
}


//recursive function to check if a string is valid
long count_valid(map<string, long> &M, string s){
    if(M.find(s) != M.end()){
        return M[s];
    }
    if(!isalpha(s[0]))
        return 1;
    vector<string>& prefixes = obtain_prefixes(s);
    long count = 0;
    for (string prefix : prefixes){
        if(s.substr(0, prefix.size()) == prefix)
           count += count_valid(M, s.substr(prefix.size()));
    }
    M[s] = count;
    return count;
}


//load dictionary from file
void load_dictionary(string line){
    stringstream ss(line);
    string word;
    while(ss >> word){
        // remove final comma
        if(word[word.size()-1] == ',')
            word = word.substr(0, word.size()-1);
        strips.insert(word);
    }
    strips.show_dict();

}


int main() {
    ifstream file("input.txt");
    string line;
    //read strips from file
    getline(file, line);
    cout << line << endl;
    load_dictionary(line);
    //blank line
    getline(file, line);
    //read and analayze combinations
    long total_valid = 0, nvalid = 0;
    //getline(file, line);
    while(getline(file, line)){
        map<string, long> M;
        long num_valid = 0;
        cout << line;
        num_valid = count_valid(M, line);
        if (num_valid > 0){
            total_valid += num_valid;
            cout << " is valid [" << num_valid << "]" << endl;
            nvalid++;
        }
        else
            cout << " is invalid" << endl;
    }
    cout << "total: " << total_valid << endl;
    cout << "valid: " << nvalid << endl;

    return 0;
}