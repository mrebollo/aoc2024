/*
    advent of code day 22 (1)
    password generation

    Given an initial code, generate 2000 passwords
    following these rules
    - multiply by 64, then mix and prune (2^8)
    - divide by 32, then mix and prune (2^5)
    - multiply by 2048, then mix and prune (2^11)
    where
    - mix: bitwise XOR between password and new value
    - prune: module 16777216 (2^24)
    
    Sum all obtained passwords after 2000 iterations
    
    Performs the operations in the order given, using bitwise desplacements
    
    Calculated arithmetically instead of iterate

*/  

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
using namespace std;


inline long long mix(long long code, long long secret){ return code ^ secret; }
inline long long prune(long long code){ return code % 16777216; }


long long new_password(long long secret){
    int next = prune(mix(secret, secret * 64));
    next = prune(mix(next, next / 32));
    next = prune(mix(next, next * 2048));
    return next;
}


int main(){

    long long pass, sum = 0;
    int nbuyers = 0;
    vector<int> price, diff;
    vector<vector<int> > market;

    ifstream inputf("input.txt");
    string line;
    while(getline(inputf, line)){
        pass = stoll(line);
        for(int i = 0; i < 2000; i++)
            pass = new_password(pass); 
        sum += pass;
    //}
    cout << "total sum: " << sum << endl;
    return 0;
}