#ifndef _IF_H
#define _IF_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class instr_fetch_2{
    public:
        void next_instr(vector<vector<string> >&IF_IF,vector<string>&i_mem,int PC, vector<vector<string> >&IF_ID);
        void upd_stall(bool update){is_stall = update;}
        bool check_stall(){return is_stall;}
    private:
        bool is_jump = false;  //enable when jumb instruction is to be performed
        bool is_fetched = false;
        bool is_stall = false; 
};

/* ======================================== FUNCTION DECLARATIONS ======================================== */

void instr_fetch_2::next_instr(vector<vector<string> >&IF_IF,vector<string>&i_mem,int PC, vector<vector<string> >&IF_ID){
    if(IF_IF[PC][2] == "1") {
        int i_fetch = stoi(IF_IF[PC][0]);
        IF_ID.push_back(vector<string>());
        IF_ID[PC+1].push_back(i_mem[i_fetch]);//instrunction to be passed.
        IF_ID[PC+1].push_back("1");//control signal 
    }
}

/* ======================================== ******************* ======================================== */

class instr_fetch_1{
    public : 
        void read_instr(vector<string>&i_mem,int PC);
        void jump_to(bool set, int PC);
        void incr_i_count(){i_count++;}
        void incr_i_fetch(int PC, vector<vector<string> > &IF_IF);
        int get_i_count(){return i_count;}
    private : 
        int i_count = 0; 
        bool is_jump = false;  //enable when jumb instruction is to be performed
        int i_fetch = -1;
};

/* ======================================== FUNCTION DECLARATIONS ======================================== */

void instr_fetch_1::read_instr(vector<string>&i_mem, int PC){
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

void instr_fetch_1::incr_i_fetch(int PC, vector<vector<string> > &IF_IF) {
    if (i_fetch == i_count-1) ; 
    else if (is_jump); 
    else {
        i_fetch++; 
        IF_IF.push_back(vector<string>());
        IF_IF[PC+1].push_back(to_string(i_fetch));  // i_fetch passed
        IF_IF[PC+1].push_back("1");  // control signal 
        IF_IF[PC+1].push_back("1");  // is_fetched passed
    }
}

void instr_fetch_1::jump_to(bool set, int PC){
    i_fetch = PC;
    is_jump = set;
}

/* ======================================== *********************** ======================================== */
#endif 