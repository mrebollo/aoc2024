/*
advent of code day 11 (2)
expand an array of integers
0 -> 1
even digits -> split in two
other -> multiply by 2024
Adapt to large iterations (75) using final recursion

*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>


//#define NITER 20
int NITER = 10;

using namespace std;
using namespace chrono;

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
void blink(long long st, int iter, long long &nstones){
    totalcalls++;
    // base case
    if(iter == NITER){
        //cout << st << " ";
        return;
    }
    // applying rules
    if(st == 0)
        blink(1, iter+1, nstones);
    else {
        int digits = num_digits(st);
        int half = pow(10,digits/2);
        if(digits > 0 && is_even(digits)){
            nstones++;
            blink(st/half, iter+1, nstones);
            blink(st%half, iter+1, nstones);
        }
        else
            blink(st*2024, iter+1, nstones);
    }
}


// blink one item
long long blink_item(long long stone){
    long long nstones = 1;
    blink(stone, 0, nstones);
    return nstones;
}


int main(int argc, char **argv){
    vector<long long> stones;
    stones = load("input.txt");
    print(stones);

    //expand the array item to item
    long long nstones = 0;
    NITER = stoi(argv[1]);
    auto start = high_resolution_clock::now();
    for(long long st : stones){
        cout << "blinking " << st << endl;
        nstones += blink_item(st);
        //cout << endl;
    }
    auto stop = high_resolution_clock::now();
    cout << "final stones: " << nstones << endl;
    cout << "Time taken: " << duration_cast<seconds>(stop - start).count() << " s" << endl;
    cout << "Total calls: " << totalcalls << endl;
    return 0;
}

