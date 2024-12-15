// desplazamiento dentro de una matrix usando punteros
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ROWS 7
#define COLS 11
#define ROWS_2 3
#define COLS_2 5

//up, right, down, left
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};

int main() {
    int m[ROWS][COLS];
    cout << "\t0  1  2  3  4  5  6" << endl;
    for(int i=0; i<ROWS; i++) {
        cout << i << ":\t";
        for(int j=0; j<COLS; j++) {
            m[i][j] = i*COLS + j;
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
    int *p = &m[3][3];
    cout << "center: " << *p << endl;
    cout << "right: " << *(p+1) << endl;
    cout << "left: " << *(p-1) << endl;
    cout << "down: " << *(p+COLS) << endl;
    cout << "up: " << *(p-COLS) << endl;
    p = &m[0][0];
    while(p <= &m[ROWS-1][COLS-1]) {
        cout << *(p++) << " ";
    }
    return 0;
}