/*
advent of code day 9 (1)
defragment disk moving complete blocks
vector manipulation
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void print(vector<int> &);

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

// find space to reallocate a block (-1 if none)
// return the index in uncompressed disk 
int find_space(vector<int> &diskmap, int block){
    int block_size = diskmap[block];
    // odd blocks are items, even blocks are holes 
    for(int hole = 1; hole < block; hole += 2)
        if(diskmap[hole] >= block_size)
            return hole;
    return -1;
}


// calculates the position of a hole in the disk
int hole_position(vector<int> &disk, vector<int> &diskmap, int hole){
    int pos = 0;
    // sum all items before the hole
    for(int i = 0; i < hole; i++)
        pos += diskmap[i];
    // increments until free space if partially occupied
    while(disk[pos] > 0)
        pos++;
    return pos;
}

// reallocate a block to a hole
void reallocate(vector<int> &disk, int *block, int *hole, int block_size){
    //move file pointed by block to the hole
    for(int i = 0; i < block_size; i++)
        swap(*(hole++), *(block--));
}


// defragment the disk
// moves complete blocks to the left-most front big enough hole
// use de comp
void defragment(vector<int> &disk, vector<int> &diskmap){
    int *block_ptr = &disk.back();
    int *hole_ptr;
    // saves the original block sizes
    vector<int> blksize = diskmap;
    /*for(int i = 1; i < blksize.size(); i++)
        blksize[i] += blksize[i-1];
      */  
    // reallocates blocks backwards
    // blocks are in odd positions
    for(int block = diskmap.size() - 1; block > 0; block -= 2){
        //find a hole big enough for block
        int hole = find_space(diskmap, block);
        if(hole > 0){
            // calculate hole position and get the pointer           
            hole_ptr = &disk[hole_position(disk, blksize, hole)];
            //move the block and update the hole size
            reallocate(disk, block_ptr, hole_ptr, diskmap[block]);
            diskmap[hole] -= diskmap[block];
            //print(disk);
        }
        // move the block pointer to the next block
        block_ptr -= (blksize[block] + blksize[block-1]);
    }
}


// get the checksum of the disk
// sum <value>*<id> for all items
// avoid -1 (it can contain holes)
long long checksum(vector<int> &disk){
    long long chk = 0;
    for(int i = 0; i < disk.size(); i++)
        if(disk[i] > 0)
            chk += disk[i] * i;
    return chk;
}

//print the disk until size 
void print(vector<int> &disk){
    for(int i = 0; i < disk.size(); i++){
        cout << ((disk[i] >= 0) ? to_string(disk[i]) : "."); 
    }
    cout << endl;
}


int main(){
    vector<int> diskmap;
    string map;
    //load the disk map from file
    fstream inputf("input.txt");
    getline(inputf, map);
    inputf.close();

    //expand format
    vector<int> disk;
    expand(map, disk);
    print(disk);

    //degfragment disk
    for(char ch : map)
        diskmap.push_back(ch-'0');
    defragment(disk, diskmap);
    cout << "-------------------" << endl;
    cout << "Defragmented disk:" << endl;
    cout << "-------------------" << endl;
    print(disk);

    //get checksum
    long long chk = checksum(disk);
    cout << "Checksum: " << chk << endl;
    return 0;
}