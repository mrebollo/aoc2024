/*
advent of code day 11 (2)
expand an array of integers
0 -> 1
even digits -> split in two
other -> multiply by 2024
Dynamic programming: include memoization in recursive function
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>


//global variable to allow change from the command line
int NITER = 75;

using namespace std;
using namespace chrono;
// memoization vector 
//memo[i][it] -> number of stones after blinking i for it iterations
vector<vector<long long> > memo(1000000);  
// stats to determine PD efficiency
int memouse = 0;
int totalcalls = 0;


// load the initial vector
vector<long long> load(string filename){
    fstream inputf(filename);
    vector<long long> v;
    string line;
    getline(inputf, line);
    stringstream ss(line);
    string num;
    while(getline(ss, num, ' '))
        v.push_back(stoi(num));
    inputf.close();
    return v;
}


// print the vector
void print(vector<long long> &v){
    for(int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

// number of digits in a number
inline int num_digits(long long n){
    //trick: add 0.1 to force 100 -> 3 digits
    return ceil(log10(n+0.1));
}

// check if a number is even
inline bool is_even(int n){
    return n % 2 == 0;
}

// blink and apply rules
long long blink(long long st, int iter){
    totalcalls++;
    // memoization
    // check if the vector is big enough
    if(st < memo.size() && memo[st][iter] != 0){
        memouse++;
        return memo[st][iter];
    }
    // base case
    if(iter == NITER){
        if( st < memo.size())
            memo[st][iter] = 1;
        return 1;
    }
    // applying rules
    long long res;    
    if(st == 0)
        res = blink(1, iter+1);
    else {
        int digits = num_digits(st);
        int half = pow(10,digits/2);
        if(digits > 0 && is_even(digits)){
            res = blink(st/half, iter+1) + blink(st%half, iter+1);
        }
        else
            res = blink(st*2024, iter+1);
    }
    if(st < memo.size())
        memo[st][iter] = res;
    return res;
}


int main(int argc, char **argv){
    vector<long long> stones;
    stones = load("input.txt");
    print(stones);

    //resize the memoization vector according to number of iterations
    if(argc > 1)
        NITER = stoi(argv[1]);
    for(auto &m : memo)
        m = vector<long long>(NITER+1, 0);

    //expand the array item to item
    long long nstones = 0;
    auto start = high_resolution_clock::now();
    for(long long st : stones){
        cout << "blinking " << st << endl;
        nstones += blink(st, 0);
    }
    auto stop = high_resolution_clock::now();
    cout << "Final stones: " << nstones << endl;
    cout << "Total calls: " << totalcalls << endl;
    cout << "Memoization used: " << memouse << endl;
    cout << "Proportion " << (double)memouse/totalcalls << endl;
    cout << "Exec time: " << duration_cast<milliseconds>(stop - start).count() << " ml" << endl;
    return 0;
}

