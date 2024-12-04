/*
advent of code day 4 (1)
find a word in a matrix (all orientations)
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

int findword(string s, string word){
    int count = 0;
    int pos = s.find(word);
    while(pos != string::npos){
        count++;
        pos = s.find(word, pos + 1);
    }
    return count;
}

//search for a word in the matrix
int search(vector<string> matrix, string word){
    int count = 0;
    for(auto x: matrix){
        //forward
        count += findword(x, word);
        //backward
        reverse(x.begin(), x.end());
        count += findword(x, word);
    }
    return count;
}


int main() {
    vector<string> matrix = load("input.txt");
    cout << "rows" << endl;
    int count = search(matrix, "XMAS");
    vector<string> t = transpose(matrix);
    cout << "columns" << endl;
    count += search(t, "XMAS");
    cout << count << endl;
    vector<string> d = diagonals(matrix);
    cout << "diagonals" << endl;
    count += search(d, "XMAS");
    vector<string> rd = rdiagonals(matrix);
    cout << "reverse diagonals" << endl;
    count += search(rd, "XMAS");
    cout << count << endl;  
    return 0;
}