#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// create a binary tree-like path
// with all posible combination of op: (+) left, (*) right
bool eval(vector<int> v, int idx, long long res, long long total){
	if(idx==v.size()){
		return res == total;
	}
	return eval(v, idx+1, res+v[idx], total ) || 
		   eval(v, idx+1, res*v[idx], total );
}


int main(){
    fstream inputf("input.txt");
    string line, data;
	vector<int> values;
	//values can exceed int limits
	long long result, sum = 0;
     while(getline(inputf, line)){
		stringstream ss(line);
		//get the result 
		getline(ss, data, ':');
		result = stoll(data);
		//get the values
		while(ss >> data)
			values.push_back(stoi(data));
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
