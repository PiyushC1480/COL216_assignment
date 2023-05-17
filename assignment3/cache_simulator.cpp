/*
ASSIGNMENT 3: COL216 
CACHE SIMULATOR 
------------------------------
SHANKH GUPTA 2021CS50604
PIYUSH CHAUHAN 2021CS110010
-----------------------------
Assumption:
1)CONSIDERED THAT L1 CACHE IS A SUBSET OF L2 CACHE.
2) if  L2 cache block needs to be evicted, then it must invalidate the block in L1.
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm>
#include <vector>

using namespace std;

//CONFIGURATIONAL Parameters
long BLOCKSIZE;
long L1_SIZE;
long L1_ASSOC;
long L2_SIZE;
long L2_ASSOC;

//counters
long L1_read = 0;
long L1_read_misses = 0;
long L1_write = 0;
long L1_write_misses = 0;
long L1_miss_rate = 0;
long L1_write_back = 0;
long L2_read = 0;
long L2_read_misses = 0;
long L2_write = 0;
long L2_write_misses = 0;
long L2_write_miss = 0;
long L2_write_back = 0;
long total_time = 0;
bool is_time=true;
string traceFile;

string::size_type sz;
//pointer to the trace file
long fp = 0;

/*------------------------------------
ADDRESS STRUCTURE
It stores the block address , index number and tag number for a particular data
--------------------------------------*/
struct address{
    string block_addr;
    string tag;
    int index;
};

/*------------------------------------
BLOCK STRUCTURE
Behaves like a block present in the Cache.
--------------------------------------*/
struct block
{
    string block_addr;
    string mem_addr;
    string tag;
    int index;
    bool valid;
    long age;
    long dirty;
};

// for LRU policy. stores the counter of newest value.
long L1age = 1;
long L2age = 1;
//numbe of sets in l1 and l2
long L1Sets;
long L2Sets;
/*------------------------------------
L1 and L2 CACHE
It is a stucture which has pointers to pointers to block
Bascically , block* is set and block** is a block
thus its a collection of sets.
--------------------------------------*/
block** L1;
block** L2;


// a trace will contain the operation type and memory address 
struct trace_struct{
    string opr;
    string mem_addr;
};

// global parameter so that all functions can access it
vector<trace_struct*> trace_data;
/*------------------------------------
this takes parameters from a line in the trace file.
creates a structure and assign the values to those stucture 
--------------------------------------*/
trace_struct* gettrace_struct(string opr, string addr)
{
    trace_struct *temp = new trace_struct();
    temp->mem_addr = addr;
    temp->opr = opr;
    return temp;
}

/*------------------------------------
GET_BITS
this funciton takes a address and calculates the calues of index, tag and set
--------------------------------------*/
address get_bits(string mem_addr, int BLOCKSIZE, int numSets){
    long int_addr = stol(mem_addr, &sz, 16);//convert memory address into string
    int block = ceil(log2(BLOCKSIZE));//get the block size
    int set = ceil(log2(numSets));// get set size
    long block_addr  = int_addr % long(pow(2, block));
    block_addr = int_addr - block_addr;
    long tag = int_addr / long(pow(2, set+block));
    int index = (int_addr % long(pow(2, set+block))) / long(pow(2, block));
    address addr;
    stringstream sstream;
    sstream << hex << tag;
    addr.tag = sstream.str();
    stringstream sstream2;
    sstream2 << hex << block_addr;
    addr.block_addr =  sstream2.str();
    addr.index =  index;
    return(addr);
}

/*------------------------------------
this funciton tells whether the data in L1 is invalid or not
--------------------------------------*/
bool L1invalid(string mem_addr){
    address addr = get_bits(mem_addr, BLOCKSIZE, L1Sets);
    for (int j=0; j<L1_ASSOC; j++){
        if (L1[addr.index][j].block_addr==addr.block_addr){
            if (L1[addr.index][j].valid == true){
                L1[addr.index][j].valid = false;
                if (L1[addr.index][j].dirty == 1){
                    return(true);
                }
                if (L1[addr.index][j].dirty == 0){
                    return(false);
                }
            }
        }
    }
    return(false);
}

/*------------------------------------
funciton to write in L2 cache
--------------------------------------*/
void L2Write(string mem_addr, int write){
    //if L2 cache size is zero then there is nothing to be done
    if (L2_SIZE==0){
        return;
    }
    //calculate the address from given hex address
    address addr = get_bits(mem_addr, BLOCKSIZE, L2Sets);
    // if it is a direct wirte command
    if (write==1){

        L2_write += 1;
        //Check if the address is available, then set the dirty bit to true and return
        for (int i=0; i<L2_ASSOC; i++){
            if (L2[addr.index][i].tag == addr.tag && L2[addr.index][i].valid==true){
                L2[addr.index][i].age = L2age++;
                L2[addr.index][i].dirty = 1;
                return;
            }
        }
        //otherwise it is a write miss
        L2_write_misses += 1;
    }
    //if it is not a direct write command
    long empty = INT_MAX;//gets the minimum valus of age bit(the least used one)
    for (int i=0; i<L2_ASSOC; i++){
        if (L2[addr.index][i].valid==false){
            empty = L2[addr.index][i].age;
            break;
        }
        else {
            empty = min(long(empty), L2[addr.index][i].age);
        }
    }
    for (int i=0; i<L2_ASSOC; i++){
        if (L2[addr.index][i].age==empty){
            // if the data to be removes id LRU
            if (L2[addr.index][i].valid==true){
                if (L2[addr.index][i].dirty==true){
                    bool r = L1invalid(L2[addr.index][i].mem_addr);
                    if (r==false){
                        L2_write_back += 1;
                    }
                }
                else{
                    L1invalid(L2[addr.index][i].mem_addr);
                }
            }
            //add the data in the L2 cache and set its parameters accordingly
            L2[addr.index][i].block_addr = addr.block_addr;
            L2[addr.index][i].index = addr.index;
            L2[addr.index][i].mem_addr = mem_addr;
            L2[addr.index][i].tag = addr.tag;
            L2[addr.index][i].age = L2age++;
            L2[addr.index][i].valid = 1;
            //if write operation then turn on dirty bit
            if (write==1){
                L2[addr.index][i].dirty = 1;
            }
            else {
                L2[addr.index][i].dirty = 0;  
            }
            break;
        }
    }
}
/*------------------------------------
funciton to read in L2 cache
--------------------------------------*/
void L2Read(string mem_addr){
    if (L2_SIZE==0){
        return;
    }
    //get the address strcutre
    address addr = get_bits(mem_addr, BLOCKSIZE, L2Sets);
    //increase the counter
    L2_read += 1;
    //search in the set for the data by matching its index and tag bits
    for (int i=0; i<L2_ASSOC; i++){
        if (L2[addr.index][i].tag == addr.tag && L2[addr.index][i].valid==true){
            L2[addr.index][i].age = L2age++;
            return;
        }
    }
    L2_read_misses += 1;
    L2Write(mem_addr, 0);
}
/*------------------------------------
funciton to write in L1 cache
similar to L2 wirte
--------------------------------------*/
void L1Write(string mem_addr, int write){
    //calculate the address from given hex address
    address addr = get_bits(mem_addr, BLOCKSIZE, L1Sets);
    // if it is a direct wirte command
    if (write==1){
        //increase the counter
        L1_write += 1;
        //search for the data in the set
        for (int i=0; i<L1_ASSOC; i++){
            if (L1[addr.index][i].tag == addr.tag && L1[addr.index][i].valid==true){
                L1[addr.index][i].age = L1age++;
                L1[addr.index][i].dirty = 1;
                return;
            }
        }
        //if not found then a write miss.
        L1_write_misses += 1;
    }
    long empty = INT_MAX;//gets the minimum valus of age bit(the least used one)
    for (int i=0; i<L1_ASSOC; i++){
        if (L1[addr.index][i].valid==false){
            empty = L1[addr.index][i].age;
            break;
        }
        else {
            empty = min(empty, L1[addr.index][i].age);
        }
    }
    for (int i=0; i<L1_ASSOC; i++){
        if (L1[addr.index][i].age==empty){
            //If empty slot
            if (L1[addr.index][i].valid==false){
            }
            else{
                if (L1[addr.index][i].dirty==1){
                    L1[addr.index][i].valid = false;
                    L1_write_back += 1;
                    // write the block which is removed in L2
                    L2Write(L1[addr.index][i].mem_addr, 1);
                }
                else{
                    L1[addr.index][i].valid = false;
                }
            }
            //get the data from l2 to l1
            L2Read(mem_addr);
            L1[addr.index][i].valid = true;
            L1[addr.index][i].mem_addr = mem_addr;
            L1[addr.index][i].block_addr = addr.block_addr;
            L1[addr.index][i].tag = addr.tag;
            L1[addr.index][i].index = addr.index;
            L1[addr.index][i].age = L1age++;
            if (write==1){
                L1[addr.index][i].dirty = 1;
            }
            else {
                L1[addr.index][i].dirty = 0;
            }
            break;
        }
    }
}
/*------------------------------------
funciton to read in L1 cache
--------------------------------------*/
void L1Read(string mem_addr){
    address addr = get_bits(mem_addr, BLOCKSIZE, L1Sets);
    L1_read += 1;
    for (int i=0; i<L1_ASSOC; i++){
        if (L1[addr.index][i].tag == addr.tag && L1[addr.index][i].valid==true){
            L1[addr.index][i].age = L1age++;
            return;
        }
    }
    L1_read_misses += 1;
    L1Write(mem_addr, 0);
}

int main(int argc, char* argv[])
{

    if (argc<6)
    {
        cout<<"insufficient arguments"<<endl;
        return(0);
    }
    BLOCKSIZE = stol( argv[1], &sz);
    L1_SIZE = stol(argv[2], &sz);
    L1_ASSOC = stol(argv[3], &sz);
    L2_SIZE = stol(argv[4], &sz);
    L2_ASSOC = stol(argv[5], &sz);
    traceFile = argv[6];

    //Create L1 Cache
    L1Sets = L1_SIZE / (BLOCKSIZE*L1_ASSOC);
    L1 = new block*[L1Sets];
    for (int i=0; i<L1Sets; i++){
        L1[i] = new block[L1_ASSOC];
    }
    //Set default values
    for (int i=0; i<L1Sets; i++){
        for (int j=0; j<L1_ASSOC; j++){
            L1[i][j].block_addr = "";
            L1[i][j].mem_addr = "";
            L1[i][j].index = i;
            L1[i][j].tag = "";
            L1[i][j].valid = false;
            L1[i][j].age = 0;
            L1[i][j].dirty = 0;
        }
    }

    //Create L2 Cache
    if (L2_SIZE>0){
        L2Sets = L2_SIZE / (BLOCKSIZE*L2_ASSOC);
        L2 = new block*[L2Sets];
        for (int i=0; i<L2Sets; i++){
            L2[i] = new block[L2_ASSOC];
        }
        //Set default values
        for (int i=0; i<L2Sets; i++){
            for (int j=0; j<L2_ASSOC; j++){
                L2[i][j].block_addr = "";
                L2[i][j].mem_addr = "";
                L2[i][j].index = i;
                L2[i][j].tag = "";
                L2[i][j].valid = false;
                L2[i][j].age = 0;
                L2[i][j].dirty = 0;
            }
        }
    }
    //Open and read trace file
    // put allt he trace file data in heap
    ifstream infile(traceFile);
    string op;
    string mem_addr;
    while(infile >> op >> mem_addr )
    {
        trace_data.push_back(gettrace_struct(op,mem_addr));
    }
    for(fp = 0; fp<trace_data.size(); fp++){
        string op = trace_data[fp]->opr;
        string mem_addr = trace_data[fp]->mem_addr;
        if (op=="r"){
            L1Read(mem_addr);
           
        }
        if (op=="w"){
            
            L1Write(mem_addr, 1);
            
        }
    }
    //print the simulation result
    float L1_miss_rate = float(L1_read_misses + L1_write_misses) / float(L1_read + L1_write);
    total_time = (L1_read + L1_write) +(L2_read + L2_write)*20 + (L2_read_misses + L2_write_miss + L2_write_back)*200; 
    cout << "\n\n===== Simulation Results =====";
    cout << "\ni. number of L1 reads:\t\t\t\t" << dec << L1_read;
    cout << "\nii. number of L1 read misses:\t\t\t" << dec <<L1_read_misses;
    cout << "\niii. number of L1 writes:\t\t\t" << dec <<L1_write;
    cout << "\niv. number of L1 write misses:\t\t\t" << dec << L1_write_misses;
    cout << "\nv. L1 miss rate:\t\t\t\t" << fixed << setprecision(4) << L1_miss_rate;
    cout << "\nvi. number of writebacks from L1 memory:\t" << dec << L1_write_back;
    if (L2_SIZE != 0)
    {
        cout << "\nvii. number of L2 reads:\t\t\t" << dec << L2_read;
        cout << "\nviii. number of L2 read misses:\t\t\t" << dec << L2_read_misses;
        cout << "\nix. number of L2 writes:\t\t\t" << dec << L2_write;
        cout << "\nx. number of L2 write misses:\t\t\t" << dec << L2_write_miss;
        cout << "\nxi. L2 miss rate:\t\t\t\t" << fixed << setprecision(4) << ((float)L2_read_misses+(float)L2_write_miss)/ (L2_read+L2_write);
        cout << "\nxii. number of writebacks from L2 memory:\t" << dec <<L2_write_back << "\n";
    }
    if(is_time)
    {
        cout<<"xiii.Total time taken: \t\t\t\t"<<total_time<<" ns"<<endl;
    }
}