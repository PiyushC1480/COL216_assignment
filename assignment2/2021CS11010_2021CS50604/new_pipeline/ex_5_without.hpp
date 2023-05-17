#include "mem_5_without.hpp"
#include "alu_5_without.hpp"
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
        vector<string> branches_not_taken_list;
        int pass = 0;
        bool is_jump = false;
        bool is_stall = false;
        int PC = 0;
        bool end_prg =  false;
};

void exe::ex(int PC, vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu, memory& MEM, vector<string>& instr_mem){
    EX_MEM.push_back(vector<int>());// adding new data to the buffer
    if (is_jump){
    }else{
        pass++;
        EX_MEM[PC+1].push_back(ID_EX[PC][5]);// mem ctrl
        EX_MEM[PC+1].push_back(ID_EX[PC][6]);// wb ctrl 
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
                i_type(ID_EX,EX_MEM,alu);
                break;
            default:
                break;
        }
    }   
}

int exe::fetch(int address){
    int val;
    ifstream file;
    string line;
    file.open("reg_5_without.txt");
    int i=0;
    while(getline(file,line)){
        if (i==address){
            val = stoi(line);
            return val;
        }
        i++;
    }
};

void exe::r_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu){
    int rd = alu.r_type(PC,EX_MEM , ID_EX[PC][0], fetch(ID_EX[PC][2]),fetch(ID_EX[PC][3]));
    EX_MEM[PC+1].push_back(ID_EX[PC][1]); // rd address to be pushed (orig file EX_MEM[PC+1].push_back(ID_EX[PC][3]) )
    EX_MEM[PC+1].push_back(0);
}

void exe::i_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, alu& alu){
    int rt = alu.i_type(PC,EX_MEM, ID_EX[PC][0],fetch(ID_EX[PC][2]), ID_EX[PC][3]); // orig: int rt = ALU.Itype(pCount, EXMEM, IDEX[pCount][0], fetchValue(IDEX[pCount][1]), IDEX[pCount][3]); //[0] = opcode, [1] = rs, [2] = rt, [3] = immediate
    EX_MEM[PC+1].push_back(ID_EX[PC][1]);// orig: //Pushing back address of rt
    EX_MEM[PC+1].push_back(0);
}

void exe::load(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(ID_EX[PC][2]); 
    int reg_addr = (ID_EX[PC][1] + fetch(ID_EX[PC][3]))  / 4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(ID_EX[PC][0]);
}

void exe::store(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM){
    EX_MEM[PC+1].push_back(fetch(ID_EX[PC][2]));
    int reg_addr = (ID_EX[PC][1] + fetch(ID_EX[PC][3])) / 4;
    EX_MEM[PC+1].push_back(reg_addr);
    EX_MEM[PC+1].push_back(ID_EX[PC][0]);
}

void exe::j_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM,  vector<string>& instr_mem){
    vector<int> temp;
    string label = label_list[ID_EX[PC][1]];
    int match, new_pc;
    for (int i = 0; i< instr_mem.size();i++){
        match = instr_mem[i].find(label);
        if (match==0){
            upd_PC(i);
        }
    }

}

void exe::branch_type(vector<vector<int> >& ID_EX, vector<vector<int> >& EX_MEM, vector<string>& instr_mem, alu& alu){
    string label = label_list[ID_EX[PC][1]];
    if (alu.branch(ID_EX[PC][0], fetch(ID_EX[PC][2]), fetch(ID_EX[PC][3]))){
        vector<int> temp;
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
        if (ID_EX[PC+1][1] != 0){
            ID_EX[PC+1] = vector<int>(7);
        }
        upd_PC(index);
        
    }else{
        branches_not_taken_list.push_back(label);
        //determine the next location of PC
        for (int i=PC; i<instr_mem.size();i++){
            string search = instr_mem[i];
            int match=search.find(label);
            //finding the match for the label
            if (match == string::npos){
                    //check whether i can be PC+1 or not
                upd_PC(i);
                return;
            }
            //else if(search.substr(0,4) == "    "){
            //     upd_PC(i);
            // }
        }

        
        // c hange the pc to next instuction just like we set it in the above one.
        // since no by pass hence no need to flush the next and net to next instruction.
    }
}

#endif