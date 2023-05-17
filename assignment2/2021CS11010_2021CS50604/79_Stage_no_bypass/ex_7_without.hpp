#include "mem_7_without.hpp"
#include "alu_7_without.hpp"
#include <vector>
#include <algorithm>
#include<iostream>
#include<fstream>

#ifndef _EX_H
#define _EX_H

using namespace std;
class exe{
    public:
        void ex(int PC, vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu, vector<string>& instr_mem);// mem to be replaced by the class name in mem.h
        void branch_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, vector<string>& instr_mem, alu& alu);
        void j_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, vector<string>& instr_mem);
        void load(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM);
        void store(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM);
        void i_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu);
        void r_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu);
        bool is_end(){return end_prg;}
        void set_end(bool set){end_prg = set;}
        vector<string> label_list;
        void upd_PC(int new_PC){PC = new_PC;}
        int return_PC(){return PC;}
        void upd_stall(bool update){is_stall = update;}
        bool check_stall(){return is_stall;}
        void set_jump(bool set){is_jump = set;}
        int instr_execut(){return pass;}//
    private:
        int pass = 0;
        bool is_jump = false;
        bool is_stall = false;
        int PC = 0;
        bool end_prg =  false;
};

void exe::ex(int PC, vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu, vector<string>& instr_mem){
    EX_MEM.push_back(vector<int>());// adding new data to the buffer
    if (is_jump){
        ;
    }else{
        pass++;
        EX_MEM[PC+1].push_back(RR_ALU[PC][5]);
        EX_MEM[PC+1].push_back(RR_ALU[PC][6]);
        switch(RR_ALU[PC][0]){
            case 0:
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 1: 
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 2: 
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 3:
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 4:
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 5:
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 6:
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 7:
                r_type(RR_ALU,EX_MEM, alu);
                break;
            case 8:
                branch_type(RR_ALU,EX_MEM, instr_mem,alu);
                break;
            case 9:
                branch_type(RR_ALU,EX_MEM, instr_mem,alu);
                break;
            case 10:
                branch_type(RR_ALU,EX_MEM, instr_mem,alu);
                break;
            case 11:
                branch_type(RR_ALU,EX_MEM, instr_mem,alu);
                break;
            case 12:
                branch_type(RR_ALU,EX_MEM, instr_mem,alu);
                break;
            case 13:
                j_type(RR_ALU,EX_MEM,instr_mem);
                break;
            case 14:
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 15:
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 16:
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 17:
                i_type(RR_ALU,EX_MEM, alu);
                break;
            case 18:
                load(RR_ALU, EX_MEM);
                break;
            case 19:
                store(RR_ALU, EX_MEM);
                break;
            case 20://end
                if(!is_jump){
                    end_prg = true;
                }
                
                break;
            default:
                cout <<" wrong type of instruction" <<endl;
                break;
        }
    }
}

void exe::r_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu){
    int rd = alu.r_type(PC,EX_MEM , RR_ALU[PC][0], RR_ALU[PC][7],RR_ALU[PC][8]);
    EX_MEM[PC+1].push_back(RR_ALU[PC][1]); // rd address to be pushed (orig file EX_MEM[PC+1].push_back(RR_ALU[PC][3]) )
    EX_MEM[PC+1].push_back(0);
    EX_MEM[PC+1].push_back(1);
}

void exe::i_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, alu& alu){
    int rt = alu.i_type(PC,EX_MEM, RR_ALU[PC][0], RR_ALU[PC][7], RR_ALU[PC][3]); // orig: int rt = ALU.Itype(pCount, EXMEM, IDEX[pCount][0], fetchValue(IDEX[pCount][1]), IDEX[pCount][3]); //[0] = opcode, [1] = rs, [2] = rt, [3] = immediate
    EX_MEM[PC+1].push_back(RR_ALU[PC][1]);// orig: //Pushing back address of rt
    EX_MEM[PC+1].push_back(0);
    EX_MEM[PC+1].push_back(1);
}

void exe::load(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(RR_ALU[PC][1]);
    int reg_addr = (RR_ALU[PC][7] + RR_ALU[PC][3])/4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(RR_ALU[PC][0]);
    EX_MEM[PC+1].push_back(1);
}

void exe::store(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(RR_ALU[PC][8]);
    int reg_addr = (RR_ALU[PC][7] + RR_ALU[PC][3])/4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(RR_ALU[PC][0]);
    EX_MEM[PC+1].push_back(1);
}

void exe::j_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM,  vector<string>& instr_mem){
    vector<int> temp;
    string label = label_list[RR_ALU[PC][1]];
    int match, new_pc;
    for (int i = 0; i< instr_mem.size();i++){
        match = instr_mem[i].find(label);
        if (match==0){
            upd_PC(i);
        }
    }
}

void exe::branch_type(vector<vector<int> >& RR_ALU, vector<vector<int> >& EX_MEM, vector<string>& instr_mem, alu& alu){
    if (alu.branch(RR_ALU[PC][0], RR_ALU[PC][7], RR_ALU[PC][8])){
        vector<int> temp;
        string label = label_list[RR_ALU[PC][3]];
        int match, new_pc, index;
        for (int i=0; i<instr_mem.size();i++){
            string search = instr_mem[i];
            match=search.find(label);
            //finding the match for the label
            if (match != string::npos){
                if (match ==0){
                    index = i;
                }
            }
        }
        upd_PC(index);
    }else{
        cout<<"no branch"<<endl;
    }
}

#endif