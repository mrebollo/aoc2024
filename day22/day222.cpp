/*
    advent of code day 22 (2)
    password generation

    Given an initial code, generate 2000 passwords 
    to obtain prices for 2000 buy orders.
    - get highest price in the first 4-changes sequence
    - identify next aparition of that price
    - get the 4-changes sequence for that aparition
    - get the highest price for all buyers in the first apparition of that sequence
    - sum all highest prices for all buyers in the first apparition of that sequence
*/  

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
using namespace std;

struct seq{
    int idx;
    vector<int> values;
}; 


inline long long mix(long long code, long long secret){ return code ^ secret; }
inline long long prune(long long code){ return code % 16777216; }


long long new_password(long long secret){
    long long next = prune(mix(secret, secret * 64));
    next = prune(mix(next, next / 32));
    next = prune(mix(next, next * 2048));
    return next;
}


void print(vector<int> v){
    for(int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}


bool gain_compare(const pair<vector<int>, int> &a, const pair<vector<int>, int> &b) {
    return a.second < b.second;
}


int main(){
    long long pass, sum = 0;
    int nbuyers = 0;
    //map with the gain for each sequence
    map<vector<int>, int> global;

    ifstream inputf("input.txt");
    string line;
    int it = 0, total = 0;
    while(getline(inputf, line)){
        map<vector<int>, int> gain;
        vector<int> prices;
        pass = stoll(line);
        //generate the prices from the pass list
        prices.push_back((int)(pass % 10));
        for(int i = 0; i < 1999; i++){
           //cout << pass << endl;
            pass = new_password(pass); 
            prices.push_back((int)(pass % 10)); 
        }

        //get the difference between prices
        vector<int> diff(prices.size());
        adjacent_difference(prices.begin(), prices.end(), diff.begin());

        //get the gain for all the sequences backwards
        for(int i = diff.size()-1; i > 2; i--){
            vector<int> seq = vector<int>(diff.begin() + i-3, diff.begin()+i+1);
            gain[seq] = prices[i];
            //cout << i << ": "; print(seq);
        }
        //check best seq -1 0 1 0 
        int aux[4] = {-1, 0, 1, 0};
        vector<int> bestseq(aux, aux + 4);
        total += gain[bestseq];
        cout << it++ << " gain: " << gain[bestseq] << " total: " << total << " | "; print(bestseq);

        //accumulate the gain for all the sequences in global
        for(auto &g: gain){
            if(global.find(g.first) == global.end())
                global[g.first] = g.second;
            else
                global[g.first] += g.second;
        }
        //vector<int>::iterator bestprice = max_element(prices.begin(),prices.end());
        //vector<int> bestseq = vector<int>(diff.begin() + *bestprice - 3, diff.begin() + *bestprice + 1); 
 
    }
    auto best = *max_element(global.begin(), global.end(), gain_compare);
/*     for(auto &g: global){
        cout << g.second << " | " << "seq: "; print(g.first);
        if(g.second >= best.second){
            cout << "---> best: "; print(g.first);
        }
    } */
    cout << "seq: "; print(best.first);
    cout << "gain: " << best.second << endl; 
    return 0;
}
