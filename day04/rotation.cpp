
/*
check rotations in a matrix
*/

#include <iostream>
#include <fstream>
#include <string>
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

//trasnspose the matrix
vector<string> transpose(vector<string> &matrix){
    vector<string> transp;
    for(int i = 0; i < matrix[0].size(); i++){
        string line;
        for(auto x: matrix){
            line += x[i];
        }
        transp.push_back(line);
    }
    return transp;
}

//extract the diagonals from the matrix
vector<string> diagonals(vector<string> matrix){
    vector<string> diags;
    for(int i = 0; i < matrix.size(); i++){
        string line;
        for(int j = 0; j < matrix.size(); j++){
            if(i + j < matrix.size()){
                line += matrix[i + j][j];
            }
        }
        diags.push_back(line);
    }
    for(int i = 1; i < matrix.size(); i++){
        string line;
        for(int j = 0; j < matrix.size(); j++){
            if(i + j < matrix.size()){
                line += matrix[j][i + j];
            }
        }
        diags.push_back(line);
    }
    return diags;
}

//extract the reverse diagonals from the matrix
vector<string> rdiagonals(vector<string> matrix){
    vector<string> diags;
    for(int i = 0; i < matrix.size(); i++){
        string line;
        for(int j = 0; j < matrix.size(); j++){
            if(i + j < matrix.size()){
                line += matrix[i + j][matrix.size() - j - 1];
            }
        }
        diags.push_back(line);
    }
    for(int i = 1; i < matrix.size(); i++){
        string line;
        for(int j = 0; j < matrix.size(); j++){
            if(i + j < matrix.size()){
                line += matrix[j][matrix.size() - i - j - 1];
            }
        }
        diags.push_back(line);
    }
    return diags;
}

void print(vector<string> matrix){
    for(auto x: matrix){
        cout << x << endl;
    }
}


int main() {
    vector<string> matrix = load("mat.txt");
    cout << "original matrix" << endl;
    print(matrix);
    vector<string> diags = diagonals(matrix);
    cout << "diagonals" << endl;
    print(diags);
    vector<string> rdiags = rdiagonals(matrix);
    cout << "diagonals reversed" << endl;
    print(rdiags);
    vector<string> tran = transpose(matrix);
    cout << "transposed matrix" << endl;
    print(tran);
    return 0;
}