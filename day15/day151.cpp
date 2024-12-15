/*
advent of code day 15 (1)
moving fishes
moves inside a grid, pushing boxes in the direction of the move
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//up, right, down, left
#define WALL '#'
#define BOX 'O'
#define EMPTY '.'
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};
string dir[4] = {"^", ">", "v", "<"};
int head_to(char c);
inline bool obstacle(char c){return c == WALL || c == BOX;}


struct robot{
        int r, c, h;
        robot(): r(0), c(0), h(0){}
        robot(int r, int c, int h): r(r), c(c), h(head_to(h)){}
        string show(){
            return "(" + to_string(r) + ", " + to_string(c) + ") - " + dir[h];
        }
        ostream& operator <<(std::ostream& out){
            out << "(" << r << ", " << c << ") - " << h;
            return out;
        }
};

class Warehouse{
    private:
        vector<string> map;
        string movement;
        robot rob;
        int rows, cols;
    public:
        Warehouse(string filename);
        void print_grid();
        void execute();
        long total_gps();
    private:
        void load_grid(fstream &fin);
        void load_movements(fstream &fin);
        void find_robot();
        char ahead();
        //char twoahead();
        void move(char c);
        void push();
        // convert into one funtion using directions
        bool pushup();
        bool pushright();
        bool pushdown();
        bool pushleft();
        long gps(int i, int j){return 100 * i + j;}
};


void Warehouse::move(char c){
    rob.h = head_to(c);
    //cout << "R: " << rob.show() << endl;
    // try to move ahead
    // if it is a box, push it
    if (ahead() == BOX)
        push();
    else if (ahead() == EMPTY){
        map[rob.r][rob.c] = '.';
        rob.r += dr[rob.h];
        rob.c += dc[rob.h];
        map[rob.r][rob.c] = '@';
    }
}


void Warehouse::push(){
    //push the boxes in the direction of the robot
    bool pushed;
    switch(rob.h){
        case UP: pushed = pushup(); break;
        case RG: pushed = pushright(); break;
        case DW: pushed = pushdown(); break;
        case LF: pushed = pushleft(); break;
    }
    // moves the robot to the new position
    // codigo repetido -> funcion
    if(pushed){
        map[rob.r][rob.c] = '.';
        rob.r += dr[rob.h];
        rob.c += dc[rob.h];
        map[rob.r][rob.c] = '@';
    }
}


bool Warehouse::pushup(){
    int next = rob.r-1;
    bool pushing = true;
    do{
        //si tras la roca hay un muro, no se puede mover
        if(map[next-1][rob.c] == WALL )
            return false; //stuck
        // si tras la roca hay un hueco, se mueve por última vez
        if(map[next-1][rob.c] == EMPTY)
            pushing = false;
        // desplaza la roca una posición hacia arriba '^'
        map[next-1][rob.c] = map[next][rob.c];
        next--; 
    }while(pushing);
    return true;
}

bool Warehouse::pushright(){
    int next = rob.c+1;
    bool pushing = true;
    //look first EMPTY space to the right
    do{
        //si tras la roca hay un muro, no se puede mover
        if(map[rob.r][next+1] == WALL )
            return false; //stuck
        // si tras la roca hay un hueco, se mueve por última vez
        if(map[rob.r][next+1] == EMPTY)
            pushing = false;
        // desplaza la roca una posición hacia la derecha '>'
        map[rob.r][next+1] = map[rob.r][next];
        next++;
    }while(pushing);
    return true;
}


bool Warehouse::pushdown(){
    int next = rob.r+1;
    bool pushing = true;
    //look first EMPTY space to the right
    do{
        //si tras la roca hay un muro, no se puede mover
        if(map[next+1][rob.c] == WALL )
            return false; //stuck
        // si tras la roca hay un hueco, se mueve por última vez
        if(map[next+1][rob.c] == EMPTY)
            pushing = false;
        // desplaza la roca una posición hacia abajo 'v'
        map[next+1][rob.c] = map[next][rob.c];
        next++;
    }while(pushing);
    return true;
}


bool Warehouse::pushleft(){
    int next = rob.c-1;
    bool pushing = true;
    //look first EMPTY space to the right
    do{
        //si tras la roca hay un muro, no se puede mover
        if(map[rob.r][next-1] == WALL )
            return false; //stuck
        // si tras la roca hay un hueco, se mueve por última vez
        if(map[rob.r][next-1] == EMPTY)
            pushing = false;
        // desplaza la roca una posición hacia la izquierda '<'
        map[rob.r][next-1] = map[rob.r][next];
        next--;
    }while(pushing);
    return true;
}






Warehouse::Warehouse(string filename){
    fstream input(filename);
    movement = "";
    load_grid(input);
    load_movements(input);
    input.close();
    find_robot();
}


void Warehouse::load_grid(fstream &fin){
    string line;
    while(getline(fin, line)){
        if(line.size() == 0)
            break;
        map.push_back(line);
        cols = line.size();
    }
    rows = map.size();
}


void Warehouse::load_movements(fstream &fin){
    string line;
     while(getline(fin, line))
        movement += line;
}

// locates the robot '@' in the map 
void Warehouse::find_robot(){
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            if(map[i][j] == '@'){
                rob.r = i; rob.c = j; 
                rob.h = head_to(movement[0]);
            }
}


void Warehouse::print_grid(){
    for(string line: map)
        cout << line << endl;
}

// returns the element that is ahead the robot
char Warehouse::ahead(){
    // ¿se puede hacer con punteros=
    int newr = rob.r + dr[rob.h];
    int newc = rob.c + dc[rob.h];
    return map[newr][newc];
}


void Warehouse::execute(){
    for(char c: movement){
        move(c);
        //print_grid();
    }
}

int head_to(char c){
    switch(c){
        case '^': return UP;
        case '>': return RG;
        case 'v': return DW;
        case '<': return LF;
    }
    return -1;
}


long Warehouse::total_gps(){
    long sum = 0;
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            if(map[i][j] == 'O')
                sum += gps(i, j);
    return sum;
}


int main(){
    Warehouse w("input.txt");
    //w.print_grid();
    w.execute();
    cout << "Sum GPS: " << w.total_gps() << endl;
    return 0;
}