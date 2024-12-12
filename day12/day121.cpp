/*
advent of code day 12 (1)
garden tiling
find contigous areas labeled with the same letter, calculate its area and perimeter
and calculate the value (sum of area * perimeter)
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct sizes{
    int area;
    int perimeter;
};
int dr[4] = {0,0,1,-1};
int dc[4] = {1,-1,0,0};


class Garden{
    private:
        vector<string> garden;
        vector<sizes> plot;
    public:
        Garden(string filename);
        void print();
        void extract_plots();
        int value();
    private:
        sizes plot_at(int i, int j);
        inline bool is_inside(int i, int j){
            return i >= 0 && i < garden.size() && j >= 0 && j < garden[i].size();
        }
};

// calculate the area and perimeter of a plot
sizes Garden::plot_at(int i, int j){
    sizes s = {0, 0};
    // get the plot type and mark as visited
    char c;
    if( (c = garden[i][j]) == '.')
        return s;
    garden[i][j] = '.';
    s.area = 1;
    for(int d = 0; d < 4; d++){
        // get adjacent coordinates
        int ni = i + dr[d];
        int nj = j + dc[d];
        //check is inside the garden and if it is the same letter
        if(is_inside(ni, nj))
            // area = sum of adjacent with same character
            if(garden[ni][nj] == c)
                // area = sum of cells with same character
                s.area += plot_at(ni, nj).area;
            else
                // perimeter = sum of adjacent with different character
                s.perimeter++;
    }
    return s;
}


// load the garden when creates the object
Garden::Garden(string filename){
    fstream input(filename);
    string line;
    while(getline(input, line))
        garden.push_back(line);
    input.close();
}


// print the garden (complete or pieces)
void Garden::print(){
    for(int i = 0; i < garden.size(); i++)
        cout << garden[i] << endl;
}


// extract the plots (areas)
void Garden::extract_plots(){
    // iterate over the garden
    for(int i = 0; i < garden.size(); i++)
        for(int j = 0; j < garden[i].size(); j++)
            // if it is a letter
            if(isalpha(garden[i][j])){
                cout << garden[i][j] << " plot at " << i << ", " << j << endl;
                sizes s = plot_at(i, j);
                print();
                cout << "Area: " << s.area << " Perimeter: " << s.perimeter << endl << endl;
                plot.push_back(s);
            }
}


// calculate the value
int Garden::value(){
    int val = 0;
    for(int i = 0; i < plot.size(); i++)
        val += plot[i].area * plot[i].perimeter;
    return val;
}


int main(){
    // load the garden
    Garden g("test10.txt");
    g.print();
    // extract the plots (areas)
    g.extract_plots();
    // calculate value
    int val = g.value();
    cout << "Value: " << val << endl;
    return 0;
}