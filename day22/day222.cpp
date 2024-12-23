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


void four_changes(vector<int> v, vector<seq> &allseqs){
    seq s;
    // begin at 1 since 0 contains no difference but value
    for(int i = 1; i < v.size()-3; i++){
        if(v[i] != 0 && v[i+1] != 0 && v[i+2] != 0 && v[i+3] != 0){
            s.values = vector<int>(v.begin() + i, v.begin() + i+4);
            s.idx = i;
            allseqs.push_back(s);
        }
    }
}

// a seq is valid if its msx appears before in the complete sequence
// or if it is the last value in the sequence
bool is_valid(seq s, vector<int> price){
    int mx = *max_element(s.values.begin(), s.values.end());
    // if it is the last
    if(mx == s.values[3])
        return true;
    // if mx appears before in the sequence
    auto it = find(price.begin(), price.end(), mx);
    return (*it < s.idx) ? true : false;
}


int sell_at_sequence(seq s, vector<vector<int> > &market){
    return 0;
}


int main(){
    long long pass, sum = 0;
    int nbuyers = 0;
    vector<int> prices, diff;
    vector<vector<int> > market;

    ifstream inputf("test.txt");
    string line;
    //while(getline(inputf, line)){
        getline(inputf, line);
        pass = stoll(line);
        //pass = 123;
        prices.push_back((int)(pass % 10));
        for(int i = 0; i < 2000; i++){
            //cout << pass << endl;
            pass = new_password(pass); 
            prices.push_back((int)(pass % 10)); 
        }
        market.push_back(prices);
        diff.resize(prices.size());
        adjacent_difference(prices.begin(), prices.end(), diff.begin());
        cout << "prices: "; print(prices);
        cout << "diff: "; print(diff);
        prices.clear();
    //}
    // buy orders: check all possible 4-changes sequences in first buyer    
    vector<seq> allseqs;
    four_changes(diff, allseqs);
    int max = 0;
    for(int i = 0; i < allseqs.size(); i++)
        if(is_valid(allseqs[i], diff)){
            int price = sell_at_sequence(allseqs[i], market);
            if(price > max)
                max = price;
        }
    cout << "max: " << max << endl;
    return 0;
}


/*
// buy orders
    // get first 4-changes sequence
    vector<int> seq(4);
    int idx = four_changes(diff, seq);
    cout << "idx: " << idx << endl;
    cout << "seq: "; print(seq);
    // get highest price in the first 4-changes sequence
    int mx = *max_element(seq.begin(), seq.end());
    cout << "max: " << mx << endl;
    // identify next aparition of that price
    auto it = find(market[0].begin(), market[0].end(), mx);
    it = find(it+1, market[0].end(), mx);
    cout << "found at it: " << *it << endl;
    // get the 4-changes sequence finishing at that aparition
    vector<int> final = vector<int>(diff.begin() + *it - 3, diff.begin() + *it + 1);
    cout << "final: "; print(final);
    return 0;
*/