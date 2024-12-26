/*
advent of code day 12 (2)
garden tiling
find contigous areas labeled with the same letter
count number of sides (= number of vertex)
- outer vertex: two neighbors in the diagonal different from the current letter
                also:  
- inner vertex (convex): some neighbor different from the current letter is an outer vertex 

Problem: a cell can have two o more vertex. Example: D in the garden below, 
or A / E in the extremes of the piece.
- 2 eq, 2 diff in 90º: 1 vertex
- 3 outer neigs: 2 vertex
- 4 outer neigs: 4 vertex


+-+-+-+-+
|A A A A|
+-+-+-+-+     +-+
              |D|
+-+-+   +-+   +-+
|B B|   |C|
+   +   + +-+
|B B|   |C C|
+-+-+   +-+ +
          |C|
+-+-+-+   +-+
|E E E|
+-+-+-+


*/  


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct sizes{
    int area;
    int inner, outer;
    sizes operator +(const sizes &s){
        return {area + s.area, inner + s.inner, outer + s.outer};
    }
    int value(){return area * (inner + outer);}
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
        bool is_inner(int i, int j);
        bool is_outer(int i, int j);
        bool diagonal(vector<pair<int, int> > neig){
            return neig.size() == 2 && (neig[0].first - neig[1].first) * (neig[0].second - neig[1].second) != 0;
        }
        inline bool is_inside(int i, int j){
            return i >= 0 && i < garden.size() && j >= 0 && j < garden[i].size();
        }
};

// check the relative position of the neighbors
// if the neighbors are in the diagonal, the product of the differences is not zero
bool diagonal(vector<pair<int, int> > neig){
    return neig.size() == 2 && 
            (neig[0].first - neig[1].first) * (neig[0].second - neig[1].second) != 0;
}

// check if a cell is an outer vertex (two neighbors in the diagonal different from the current letter)
bool Garden::is_outer(int i, int j){
    char c = garden[i][j];
    // using a vector for the i,j of each neighbor
    vector<pair<int, int> > neig;
    for(int d = 0; d < 4; d++){
        int ni = i + dr[d];
        int nj = j + dc[d];
        if(is_inside(ni, nj) && garden[ni][nj] != c)
            neig.push_back(make_pair(ni, nj));
    }
    return diagonal(neig) || neig.size() >= 3;
}


// check if a cell is an inner vertex (the neighbor is outer)
bool Garden::is_inner(int i, int j){
    char c = garden[i][j];
    pair<int, int> neig(-1, -1);
    for(int d = 0; d < 4; d++){
        int ni = i + dr[d];
        int nj = j + dc[d];
        if(is_inside(ni, nj) && garden[ni][nj] != c)
            if(neig.first == -1)
                neig = make_pair(ni, nj);
            else
                return false;
    }
    // arrives here if there is only one 4-neighbour
    // TODO: review formulae in the borders
    return is_outer(neig.first, neig.second );
}


// calculate the area and perimeter of a plot
sizes Garden::plot_at(int i, int j){
    sizes s = {0, 0};
    // get the plot type
    char c;
    if( (c = garden[i][j]) == '-'){
        return s;
    }
    s.area = 1;
    if(is_outer(i, j))
        s.outer = 1;
    else if(is_inner(i, j))
        s.inner = 1;
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
                print();
                cout << current << " Area: " << s.area << " Inner: " << s.inner << " Outer: " << s.outer << endl << endl;
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
    Garden g("test4.txt");
    g.print();
    // extract the plots (areas)
    g.extract_plots();
    // calculate value
    int val = g.value();
    cout << "Value: " << val << endl;
    return 0;
}