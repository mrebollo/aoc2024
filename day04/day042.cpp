/*
advent of code day 4 (2)
find word MAS in a matrix (diagonal crossing)
M.S
.A.
M.S
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


//load the matrix from a file
vector<string> load(string filename){
    ifstream file(filename);
    string line;
    vector<string> matrix;
    while(getline(file, line)){
        matrix.push_back(line);
    }
    return matrix;
}


//check if the word MAS is in the matrix at (i,j)
//called if there is an 'A' in (i,j), check the rest
bool is_MAS(vector<string> m, int i, int j){
    string d1 = string() + m[i-1][j-1] + m[i][j] + m[i+1][j+1];
    string d2 = string() + m[i-1][j+1] + m[i][j] + m[i+1][j-1];
    return (d1 == "MAS" || d1 == "SAM") && (d2 == "MAS" || d2 == "SAM");
}

//search for word MAS in the matrix
int searchMAS(vector<string> matrix){
    int count = 0;
    // find 'A' in central positions (avoiding the borders)
    for(int i = 1; i < matrix.size()-1; i++)
        for(int j = 1; j < matrix.size()-1; j++)
            if(matrix[i][j] == 'A' && is_MAS(matrix, i, j))
                count++;
    return count;
}


int main() {
    vector<string> matrix = load("input.txt");
    int count = searchMAS(matrix);
    cout << count << endl;  
    return 0;
}