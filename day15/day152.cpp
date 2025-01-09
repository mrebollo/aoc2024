
/*
advent of code day 15 (2)
moving fishes
moves inside a grid, pushing boxes in the direction of the move
Double the size of obstacles and boxes
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
#define DBOX "[]"
enum dir {UP, RG, DW, LF}; 
int dr[4] = {-1, 0, 1, 0}; 
int dc[4] = {0, 1, 0, -1};
string dir[4] = {"^", ">", "v", "<"};
int head_to(char c);
inline bool obstacle(char c){return c == WALL || c == DBOX[0] || c == DBOX[1];}





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
        int count_boxes();
    private:
        void load_grid(fstream &fin);
        void load_movements(fstream &fin);
        void find_robot();
        char ahead();
        void move(char c);
        void push();
        // convert into one funtion using directions
        bool pushup(int r, int c1, int c2);
        bool pushright(int r, int c);
        bool pushdown(int r, int c1, int c2);
        bool pushleft(int r, int c);
        void boundingbox(int r, int &c1, int &c2);
        long gps(int i, int j){return 100 * i + j;}
};



void Warehouse::move(char c){
    rob.h = head_to(c);
    //cout << "R: " << rob.show() << endl;
    // try to move ahead
    // if it is a box, push it
    if (ahead() == DBOX[0] || ahead() == DBOX[1])
        push();
    // move is it's empty or after pushing
    if (ahead() == EMPTY){
        map[rob.r][rob.c] = '.';
        rob.r += dr[rob.h];
        rob.c += dc[rob.h];
        map[rob.r][rob.c] = dir[rob.h][0];
    }
}


void Warehouse::push(){
    bool pushed = false, vert = false;
    //push the boxes in the direction of the robot
    switch(rob.h){
        case UP: pushed = pushup(rob.r, rob.c, rob.c); break;
        case RG: pushed = pushright(rob.r, rob.c); break;
        case DW: pushed = pushdown(rob.r, rob.c, rob.c); break;
        case LF: pushed = pushleft(rob.r, rob.c); break;
    }
    // moves the robot to the new position
    if (pushed){
        map[rob.r][rob.c] = '.';
        rob.r += dr[rob.h];
        rob.c += dc[rob.h];
        map[rob.r][rob.c] = dir[rob.h][0];//'@'
    }
}


bool Warehouse::pushup(int r, int c1, int c2){
    // caso base: posición libre tras la caja
    if(map[r-1][c1] == EMPTY && map[r-1][c2] == EMPTY){
        return true;
    }
    // caso base 2: muro tras la caja
    if(map[r-1][c1] == WALL || map[r-1][c2] == WALL)
        return false;
    // caso general (hay otra caja tras la caja)
    // busca los bordes de la caja
    // tres casos: apoyo izda, apoyo dcha, apoyo en ambos lados
    bool pushed = true;
    // caso 1: apoyo completo (caja de 2 posiciones)
    // cuando empuja el robot es caso 2 o 3
    if(map[r-1][c1] == DBOX[0] && map[r-1][c2] == DBOX[1])
        pushed = pushup(r-1, c1, c2);
    else{
        // caso 2: apoyo izda
        if(map[r-1][c1] == DBOX[1])
            pushed = pushup(r-1, c1-1, c1);
        // caso 3: apoyo dcha
        if(map[r-1][c2] == DBOX[0])
            pushed &= pushup(r-1, c2, c2+1);
    }
    // si se ha podido mover la caja, 
    /// se mueven las cajas que están detrás
    if(pushed){
        if(map[r-1][c1] == DBOX[0] && map[r-1][c2] == DBOX[1]){
            //mueve la caja apoyada por completo
            map[r-2][c1] = map[r-1][c1];
            map[r-2][c2] = map[r-1][c2];
            map[r-1][c1] = '.';
            map[r-1][c2] = '.';
        }
        else{
            //mueve la caja apoyada por la izda
            if(map[r-1][c1] == DBOX[1]){
                map[r-2][c1] = map[r-1][c1];
                map[r-2][c1-1] = map[r-1][c1-1];
                map[r-1][c1] = '.';
                map[r-1][c1-1] = '.';
            }
            //mueve la caja apoyada por la dcha
            if(map[r-1][c2] == DBOX[0]){
                map[r-2][c2] = map[r-1][c2];
                map[r-2][c2+1] = map[r-1][c2+1];
                map[r-1][c2] = '.';
                map[r-1][c2+1] = '.';
            }
        }
    }
    return pushed;
}


bool Warehouse::pushdown(int r, int c1, int c2){
    //caso base: posición libre tras la caja
    if(map[r+1][c1] == EMPTY && map[r+1][c2] == EMPTY){
        return true;
    }
    //caso base 2: muro tras la caja
    if(map[r+1][c1] == WALL || map[r+1][c2] == WALL)
        return false;
    //caso general (hay otra caja tras la caja)
    //busca los bordes de la caja
    //tres casos: apoyo izda, apoyo dcha, apoyo en ambos lados
    bool pushed = true;
    //caso 1: apoyo completo (caja de 2 posiciones)
    //cuando empuja el robot es caso 2 o 3
    if(map[r+1][c1] == DBOX[0] && map[r+1][c2] == DBOX[1])
        pushed = pushdown(r+1, c1, c2);
    else{
        //caso 2: apoyo izda
        if(map[r+1][c1] == DBOX[1])
            pushed = pushdown(r+1, c1-1, c1);
        //caso 3: apoyo dcha
        if(map[r+1][c2] == DBOX[0])
            pushed &= pushdown(r+1, c2, c2+1);
    }   
    //si se ha podido mover la caja,
    //se mueven las cajas que están detrás
    if(pushed){
        if(map[r+1][c1] == DBOX[0] && map[r+1][c2] == DBOX[1]){
            //mueve la caja apoyada por completo
            map[r+2][c1] = map[r+1][c1];
            map[r+2][c2] = map[r+1][c2];
            map[r+1][c1] = '.';
            map[r+1][c2] = '.';
        }
        else{
            //mueve la caja apoyada por la izda
            if(map[r+1][c1] == DBOX[1]){
                map[r+2][c1] = map[r+1][c1];
                map[r+2][c1-1] = map[r+1][c1-1];
                map[r+1][c1] = '.';
                map[r+1][c1-1] = '.';
            }
            //mueve la caja apoyada por la dcha
            if(map[r+1][c2] == DBOX[0]){
                map[r+2][c2] = map[r+1][c2];
                map[r+2][c2+1] = map[r+1][c2+1];
                map[r+1][c2] = '.';
                map[r+1][c2+1] = '.';
            }
        }
    }
    return pushed;
}


//TODO: refactor pushright and pushleft into one function
//     using heading to determine the direction
//     heading == 1 sum, heading == 3 substract (multiply by -1) 
bool Warehouse::pushright(int r, int c){
    // caso base: posicion libre tras la caja
    if(map[r][c+3] == EMPTY){
        //TODO try to remove this code duplication
        map[r][c+3] = map[r][c+2];
        map[r][c+2] = map[r][c+1];
        map[r][c+1] = map[r][c];
        return true;
    }
    // caso base 2: muro tras la caja
    if(map[r][c+3] == WALL)
        return false;
    // caso general (hay otra caja tras la caja)
    bool pushed = pushright(r, c+2);
    if(pushed){
        map[r][c+3] = map[r][c+2];
        map[r][c+2] = map[r][c+1];
        map[r][c+1] = map[r][c];
    }
    return pushed;
}


bool Warehouse::pushleft(int r, int c){
    //caso base: posición libre tras la caja
    if(map[r][c-3] == EMPTY){
        // create inline function to do this
        map[r][c-3] = map[r][c-2];
        map[r][c-2] = map[r][c-1];
        map[r][c-1] = map[r][c];
        return true;
    }
    //caso base 2: muro tras la caja
    if(map[r][c-3] == WALL)
        return false;
    //caso general (hay otra caja tras la caja)
    bool pushed = pushleft(r, c-2);
    if(pushed){
        map[r][c-3] = map[r][c-2];
        map[r][c-2] = map[r][c-1];
        map[r][c-1] = map[r][c];
    }
    return pushed;
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
    string line="", input;
    while(getline(fin, input)){
        if(input.size() == 0)
            break;
        for(char &c: input){
            if(c == BOX) line += DBOX;
            else if(c == WALL){ 
                line += WALL;
                line += WALL;
            }
            else if(c == '@'){
                line += '@';
                line += '.';
            }
            else{ 
                line += EMPTY;
                line += EMPTY;
            }
        }
        map.push_back(line);
        cols = input.size();
        line = "";
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
        for(int j = 0; j < cols*2; j++)
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
    int count = 0;
    for(char c: movement){
        move(c);
        //system("clear");
        //print_grid();
        count++;
    }
    cout << "Movements: " << count << endl;
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
    long sum = 0, count = 0;
    for(int i = 0; i < 2*rows; i++)
        for(int j = 0; j < 2*cols; j++)
            if(map[i][j] == '['){
                cout << "Box " << ++count << ": " << i << ", " << j << ": " << gps(i, j);
                sum += gps(i, j);
                cout  << "(" << sum << ")" << endl;
            }
    return sum;
}


int Warehouse::count_boxes(){
    int count = 0;
    for(int i = 0; i < 2*rows; i++)
        for(int j = 0; j < 2*cols; j++)
            if(map[i][j] == DBOX[0])
                count++;
    return count;
}


int main(){
    Warehouse w("input.txt");
    w.print_grid();
    w.execute();
    w.print_grid();
    cout << "Sum GPS: " << endl;
    cout << "Boxes: " << w.count_boxes() << endl;
    cout << w.total_gps() << endl;
    return 0;
}