/*
advent of code day 7 (2)
calculate the result of an operation
trying all posibble combinations of (+) (*) and (||)
where || concatenates the numbers
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// create a binary tree-like path
// with all posible combination of op: (+) (*) (||)
// where || concatenates the numbers
bool eval(vector<string> v, int idx, long long res, long long total){
	if(idx==v.size()){
		return res == total;
	}
	return eval(v, idx+1, res+stoi(v[idx]), total ) || 
		   eval(v, idx+1, res*stoi(v[idx]), total ) ||
		   eval(v, idx+1, stoll(to_string(res)+v[idx]), total) ||
		   eval(v, idx+1, stoll(to_string(res)+v[idx]), total);
}


int main(){
    fstream inputf("input.txt");
    string line, data;
	vector<string> values;
	//values can exceed int limits
	long long result, sum = 0;
     while(getline(inputf, line)){
		stringstream ss(line);
		//get the result 
		getline(ss, data, ':');
		result = stoll(data);
		//get the values
		while(ss >> data)
			values.push_back(data);
		//check if the result is possible
        if(eval(values, 0, 0, result)){
				sum += result;
				cout << line << endl;
		}
		values.clear();
    }
    inputf.close();
	cout << sum << endl;
    return 0;

}
