/*
advent of code day 9 (1)
defragment disk
vector manipulation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// expand the condensed disk map to a full disk map
// 12345 -> 0..111....22222 [<item><void>]*
void expand(string diskmap, vector<int> &disk){
    int id = 0, i;
    for(i = 0; i < diskmap.size()-1; ){
        disk.insert(disk.end(), diskmap[i++]-'0', id++);
        disk.insert(disk.end(), diskmap[i++]-'0', -1);
    }
    disk.insert(disk.end(), diskmap[i]-'0', id);
}

// defragment the disk
//insert items from the back in the holes from the front
// 0..111....22222 -> 02211122......
int  defragment(vector<int> &disk){
    int *last = &disk.back();
    int *cur = &disk.front();
    while(cur != last){
        if(*cur >= 0)
            cur++;
        else if(*last < 0)
            last--;
        else
            *(cur)++ = *(last)--;
    }
    return cur - &disk.front() + 1;
}

// get the checksum of the disk
// sum <value>*<id> for all items
long long  checksum(vector<int> disk, int size){
    long long  chk = 0;
    for(int i = 0; i < size; i++){
        chk += disk[i] * i;
    }
    return chk;
}

//print the disk until size 
void print(vector<int> disk, int size){
    for(int i = 0; i < size; i++){
        cout << ((disk[i] >= 0) ? to_string(disk[i]) : "."); 
    }
    cout << endl;
}


int main(){
    string diskmap;
    //load the disk map from file
    fstream inputf("input.txt");
    getline(inputf, diskmap);
    inputf.close();

    //expand format
    vector<int> disk;
    expand(diskmap, disk);
    //print(disk, disk.size());

    //degfragment disk
    int newsize = defragment(disk);
    //print(disk, newsize);

    //get checksum
    long long chk = checksum(disk, newsize);

    cout << "Checksum: " << chk << endl;


    return 0;
}