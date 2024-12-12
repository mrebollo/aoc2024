/*
advent of code day 12 (2)
garden tiling
find contigous areas labeled with the same letter
change perimeter by number of sides of the shape
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
    sizes operator +(const sizes &s){
        return {area + s.area, perimeter + s.perimeter};
    }
    int value(){return area * perimeter;}
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
    // get the plot type
    char c;
    if( (c = garden[i][j]) == '-'){
        return s;
    }
    s.area = 1;
    s.perimeter = 4;
    for(int d = 0; d < 4; d++){
        // get adjacent coordinates
        int ni = i + dr[d];
        int nj = j + dc[d];
        if(is_inside(ni, nj) && (garden[ni][nj] == c || garden[ni][nj] == '-'))
            s.perimeter--;
    }
    // mark the cell as visited
    garden[i][j] = '-';	
    // iterate over the 4 directions
    for(int d = 0; d < 4; d++){
        // get adjacent coordinates
        int ni = i + dr[d];
        int nj = j + dc[d];
        //check is inside the garden and if it is the same letter
        if(is_inside(ni, nj) && garden[ni][nj] == c){
            // area = sum of cells with same character
            s = s + plot_at(ni, nj);
        }
    }
    //mark cell as processed
    garden[i][j] = '.';	
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
                char current = garden[i][j];
                //cout << current << " plot at " << i << ", " << j << endl;
                sizes s = plot_at(i, j);
                //print();
                //cout << current << " Area: " << s.area << " Perimeter: " << s.perimeter << " -> " << s.value() << endl << endl;
                plot.push_back(s);
            }
}


// calculate the value
int Garden::value(){
    int val = 0;
    for(sizes s : plot)
        val += s.value();
    return val;
}


int main(){
    // load the garden
    Garden g("input.txt");
    //g.print();
    // extract the plots (areas)
    g.extract_plots();
    // calculate value
    int val = g.value();
    cout << "Value: " << val << endl;
    return 0;
}