#include "mem.hpp"
#include "alu.hpp"
#include <vector>
#include <algorithm>
#include<iostream>
#include<fstream>


#ifndef _EX_H
#define _EX_H

using namespace std;
class exe{
    public:
        void ex(int PC, vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu, memory& MEM, vector<string>& instr_mem);// mem to be replaced by the class name in mem.h
        void branch_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, vector<string>& instr_mem, alu& alu);
        void j_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, vector<string>& instr_mem);
        void load(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM);
        void store(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM);
        vector<string> branches_not_taken_list_get(){return branches_not_taken_list;}
        void i_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu);
        void r_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu);
        int fetch(int address);
        bool is_end(){return end_prg;}
        bool set_end(bool set){end_prg = set;}
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
        vector<string> branches_not_taken_list;
};

void exe::ex(int PC, vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu, memory& MEM, vector<string>& instr_mem){
    EX_MEM.push_back(vector<int>());// adding new data to the buffer
    if (is_jump){
    }else{
        pass++;
        EX_MEM[PC+1].push_back(ID_EX[PC][5]);
        EX_MEM[PC+1].push_back(ID_EX[PC][6]);
        switch(ID_EX[PC][0]){
            case 0:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 1: 
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 2: 
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 3:
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 4:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 5:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 6:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 7:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 8:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 9:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 10:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 11:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 12:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 13:
                j_type(ID_EX,EX_MEM,instr_mem);
                break;
            case 14:
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 15:
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 16:
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 17:
                i_type(ID_EX,EX_MEM, alu);
                break;
            case 18:
                load(ID_EX, EX_MEM);
                break;
            case 19:
                store(ID_EX, EX_MEM);
                break;
            case 20://end
                if(!is_jump){
                    end_prg = true;
                }
                break;
            case 21:
                branch_type(ID_EX,EX_MEM, instr_mem,alu);
                break;
            case 22:
                r_type(ID_EX,EX_MEM, alu);
                break;
            case 23:
                i_type(ID_EX,EX_MEM, alu);
                break;
            default:
                break;
        }
    }   
}



void exe::r_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu){
    int rd = alu.r_type(PC,EX_MEM , ID_EX[PC][0], ID_EX[PC][2],ID_EX[PC][3]);
    if (ID_EX[PC+1][2] == 32760){
        ID_EX[PC+1][2] = rd;
    }
    if (ID_EX[PC+1][3] == 32761){
        ID_EX[PC+1][3] = rd;
    }
    EX_MEM[PC+1].push_back(ID_EX[PC][1]); // rd address to be pushed (orig file EX_MEM[PC+1].push_back(ID_EX[PC][3]) )
    EX_MEM[PC+1].push_back(0);
}

void exe::i_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu){
    int rt = alu.i_type(PC,EX_MEM, ID_EX[PC][0],ID_EX[PC][2], ID_EX[PC][3]); // orig: int rt = ALU.Itype(pCount, EXMEM, IDEX[pCount][0], fetchValue(IDEX[pCount][1]), IDEX[pCount][3]); //[0] = opcode, [1] = rs, [2] = rt, [3] = immediate
    if (ID_EX[PC+1][2] == 32760){
        ID_EX[PC+1][2] = rt;}
    if (ID_EX[PC+1][3] == 32761){
        ID_EX[PC+1][3] = rt;
    }
    EX_MEM[PC+1].push_back(ID_EX[PC][1]);// orig: //Pushing back address of rt
    EX_MEM[PC+1].push_back(0);
}

void exe::load(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(ID_EX[PC][2]);
    int reg_addr = (ID_EX[PC][3] + ID_EX[PC][1])/4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(ID_EX[PC][0]);
}

void exe::store(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(ID_EX[PC][2]);
    int reg_addr = (ID_EX[PC][3] + ID_EX[PC][1])/4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(ID_EX[PC][0]);
}

void exe::j_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM,  vector<string>& instr_mem){
    string label = label_list[ID_EX[PC][1]];
    ID_EX[PC+1] = vector<int>(7);
    int match, new_pc;
    for (int i = 0; i< instr_mem.size();i++){
        match = instr_mem[i].find(label);
        if (match==0){
            upd_PC(i);
        }
    }
    branches_not_taken_list.clear();
}

void exe::branch_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, vector<string>& instr_mem, alu& alu){
    if(ID_EX[PC+1][2] == 32762){
        ID_EX[PC+1][2] = EX_MEM[PC][2];
    }
    if(ID_EX[PC+1][3] == 32763){
        ID_EX[PC+1][3] = EX_MEM[PC][2];
    }
    string label = label_list[ID_EX[PC][1]];
    if (alu.branch(ID_EX[PC][0], ID_EX[PC][2], ID_EX[PC][3])){
        vector<int> temp;
        auto iter = find(branches_not_taken_list.begin(), branches_not_taken_list.end(), label);
            if (branches_not_taken_list.size() != 0){
                if (iter != branches_not_taken_list.end()){
                    branches_not_taken_list.erase(iter);
                } 
            }   

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
        EX_MEM[PC+1] = vector<int>(5);
        branches_not_taken_list.push_back(label);
        //determine the next location of PC
        for (int i=PC-1; i<instr_mem.size();i++){
            string search = instr_mem[i];
            int match=search.find(label);
            //finding the match for the label
            if (match == string::npos){
                    //check whether i can be PC+1 or not
                if (i==PC-1){
                    return;
                }else{
                    upd_PC(i);
                    return;
                }
                
            }
        }
    }
}
#endif