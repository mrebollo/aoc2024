/*
advent of code day 11 (2)
expand an array of integers
0 -> 1
even digits -> split in two
other -> multiply by 2024
Adapt to large iterations (75)
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define NITER 75


using namespace std;

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
vector<long long> blink(vector<long long> &stones){
    vector<long long> newstones;
    for(int i = 0; i < stones.size(); i++){
        if(stones[i] == 0)
            newstones.push_back(1);
        else {
            int digits = num_digits(stones[i]);
            int half = pow(10,digits/2);
            if(digits > 0 && is_even(digits)){
                newstones.push_back(stones[i] / half);
                newstones.push_back(stones[i] % half);
            }
            else
                newstones.push_back(stones[i]*2024);
        }
    }
    return newstones;
}


// blink one item
int blink_item(int stone){
    vector<long long> stones(1,stone);
    for(int i = 0; i < NITER; i++)
        stones = blink(stones);
    return stones.size();
}


int main(){
    vector<long long> stones;
    stones = load("input.txt");
    print(stones);

    //expand the array item to item
    int nstones = 0;
    for(int st : stones){
        cout << "blinking " << st << endl;
        nstones += blink_item(st);
    }
    cout << "final stones: " << nstones << endl;
    return 0;
}

