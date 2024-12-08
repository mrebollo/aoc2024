#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// create a binary tree-like path
// with all posible combination of op: (+) left, (*) right
bool eval(vector<int> v, int idx, int res, int total){
	if(idx==v.size())
		return res == total;
	idx++;
	return eval(v, idx, res+v[idx], total ) || 
		    eval(v, idx, res*v[idx], total );
}


//evaluate the expression to check
//if there is an operation that gives result
bool is_equal_to(string val, int result){
	vector<int> v;
	stringstream iss(val);
	string data;
	// generate vector from string
    while (iss >> data){
        v.push_back(stoi(data));
    }
   //check all combinations of + * operators
	return eval(v, 0, v[0], result);
}

int main(){
    fstream inputf("test.txt");
    string line, values;
	int result, sum = 0;
    while(getline(inputf, line)){
        sscanf(line, "%d:%s", &result, values);
        if(is_equal_to(values, result)){
				sum += result;
				cout << line << endl;
			}
    }
    inputf.close();
	cout << result << endl;
    return 0;

}
