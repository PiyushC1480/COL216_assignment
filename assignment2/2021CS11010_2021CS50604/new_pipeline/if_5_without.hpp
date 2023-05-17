#ifndef _IF_H
#define _IF_H
/* IF_ID is the buffer register btw the two stages*/
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class instr_fetch{
    public:
        void read_instr(vector<string>&i_mem,unsigned PC);
        void next_instr(vector<vector<string> >&IF_ID,vector<string>&i_mem,unsigned PC); // dont know the meaning of this 
        void incr_i_count(){i_count++;}
        unsigned get_i_count(){return i_count;}
        void i_upd(unsigned PC);
    private:
        bool is_jump = false;  //enable when jumb instruction is to be performed
        unsigned i_count = 0;
        unsigned i_fetch = 0;

};

/*----function declarations-----*/
void instr_fetch::read_instr(vector<string>&i_mem, unsigned PC){
    ifstream instr_file;
    string instr;
    instr_file.open("input.asm");
    int line = 0;
    while(getline(instr_file,instr)){
        i_mem.push_back(instr);
        incr_i_count();
    }
    i_mem.push_back("end ");
    incr_i_count();
    // for (auto elem : i_mem) {
    //     cout << elem << ",";
    // }
    // cout << endl;
    instr_file.close();
};

void instr_fetch::next_instr(vector<vector<string> >&IF_ID,vector<string>&i_mem,unsigned PC){
    if (i_fetch==i_count){
    }
    else if (is_jump){
    }
    else{
        IF_ID.push_back(vector<string>());
        IF_ID[PC+1].push_back(i_mem[i_fetch]);//instrunction to be passed.
        IF_ID[PC+1].push_back("1");//control signal 
        i_fetch++;
    }
}

void instr_fetch::i_upd( unsigned PC){
    i_fetch = PC;
}

#endif 