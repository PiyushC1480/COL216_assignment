#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

#ifndef _ID_H
#define _ID_H

using namespace std;

class instr_decode_1{
    public:
        void decoder(vector<vector<string> >&IF_ID, vector<vector<string> >&ID_ID,int PC);
        void upd_PC(int new_PC){PC = new_PC;}
        void code_search(vector<string>& opr_name,int& r);//since ID_RR is taken to be vector of vector if int hence all the operations written ins tring will be converted into their numberic(position in the opr_list) and then stored in ID_RR;
        void upd_stall(bool update){is_stall = update;}
        bool check_stall(){return is_stall;}

    private:
        int PC = 0;
        bool is_stall = false;
        bool is_jump = false;
        string vec_to_str(vector<string>& vec);
        string operations[21] = {"add","sub","sll","srl","slt","and","or","nor","beq","bne","bgt","ble","bge","j","andi","ori","subi","addi","lw","sw", "end"};
        int pos; // processing the string ine by one by reading the elements in it;
};

// ====================================== FUNCTION DECLARATIONS ========================================= //

void instr_decode_1::decoder(vector<vector<string> >&IF_ID, vector<vector<string> >&ID_ID,int PC){
    upd_PC(PC);
    //processing the current instruction
    if (is_jump) {printf("%s", "jump exectued");}
    else{
        string instr= IF_ID[PC][0];//fetch the instruction to be decoded
        vector<string> cmd;//store the instruction insepertion
        int op = 0;
        pos = 0;
        string conv;
        bool iter = false;
        while (!iter){
            conv = instr[pos];
            if (conv == " "){
                code_search(cmd,op);
                cmd.clear();
                iter = true;
            }
            if (conv == ":"){
                string compact = vec_to_str(cmd);
                cmd.clear();
                pos+=2;
            }
            else{
                cmd.push_back(conv);
                pos++;
            }
        }
        conv = instr[pos];
        if (conv == " "){pos++;}
        string delim;
        cmd.clear();
        ID_ID.push_back(vector<string>());
        ID_ID[PC+1].push_back(to_string(op));
        ID_ID[PC+1].push_back(instr);
        ID_ID[PC+1].push_back(to_string(pos));
        ID_ID[PC+1].push_back(IF_ID[PC][1]);
    }
}

string instr_decode_1::vec_to_str(vector<string>& vec){
    string r = "";
    for (int i= 0;i<vec.size(); i++){
        r.append(vec[i]);
    }
    return r;
}

void instr_decode_1::code_search(vector<string>& opr_name,int& r){
    string opr_str = vec_to_str(opr_name);
    r = 0;
    while (opr_str != operations[r]){
        r++;
    }
}
// ------------------------------------------ ********************** -------------------------------- //


class instr_decode_2{
    public : 
        void OP_code(vector<vector<string> > & ID_ID, int PC, vector<vector<int> > & ID_RR, vector<string> &bypass);
        void upd_PC(int new_PC){PC = new_PC;}
        int opr_types(int opr);
        void data_hazards(vector<vector<int> >& ID_RR, vector<string> &bypass);
        void jump_instr(vector<vector<int> >&ID_RR, string instr, vector<vector<string> > &ID_ID);
        void branch_instr(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID);
        void load_store_instr(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID);
        void i_type(vector<vector<int> >&ID_RR, string instr, vector<vector<string> > &ID_ID);
        void r_type(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID);
        void reg_search(vector<string>& opr_name, int& r, int reg_count);
        vector<string> label_list;
        void upd_stall(bool update){is_stall = update;}
        bool check_stall(){return is_stall;}
        void set_jump(bool set){is_jump = set;}
        int check_stall_cycle(){return cycles_in_stall;}
        void reset_stall_cycle(){cycles_in_stall = 0;}

    private : 
        int PC = 0;
        bool is_jump = false;
        string vec_to_str(vector<string>& vec);
        string operations[21] = {"add","sub","sll","srl","slt","and","or","nor","beq","bne","bgt","ble","bge","j","andi","ori","subi","addi","lw","sw", "end"};
        string reg_names_1[32] =  {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
        string reg_names_2[32] =  {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31"};
        int reg_no , code_no;
        bool is_stall = false;
        int cycles_in_stall = 0;
};

// ====================================== FUNCTION DECLARATIONS ======================================== //

void instr_decode_2::OP_code(vector<vector<string> > & ID_ID, int PC, vector<vector<int> > & ID_RR, vector<string> &bypass){
    upd_PC(PC);
    bool end = false;
    int op = stoi(ID_ID[PC][0]);
    string instr = ID_ID[PC][1];
    ID_RR.push_back(vector<int>());
    ID_RR[PC+1].push_back(op);
    switch(op){
        case 0: //add
            r_type(ID_RR,instr, ID_ID);
            break;
        case 1: // sub
            r_type(ID_RR,instr, ID_ID);
            break;
        case 2: //sll
            i_type(ID_RR,instr, ID_ID);
            break;
        case 3: //srl 
            i_type(ID_RR,instr, ID_ID);
            break;
        case 4://slt
            r_type(ID_RR,instr, ID_ID);
            break;
        case 5://and
            r_type(ID_RR,instr, ID_ID);
            break;
        case 6://or
            r_type(ID_RR,instr, ID_ID);
            break;
        case 7://nor
            r_type(ID_RR,instr, ID_ID);
            break;
        case 8://beq
            branch_instr(ID_RR,instr, ID_ID);
            break;
        case 9://bne
            branch_instr(ID_RR,instr, ID_ID);
            break;
        case 10://bgt
            branch_instr(ID_RR,instr, ID_ID);
            break;
        case 11://ble
            branch_instr(ID_RR,instr, ID_ID);
            break;
        case 12://bge
            branch_instr(ID_RR,instr, ID_ID);
            break;
        case 13://j, ID_ID
            jump_instr(ID_RR,instr, ID_ID);
            break;
        case 14://andi
            i_type(ID_RR,instr, ID_ID);
            break;
        case 15://ori
            i_type(ID_RR,instr, ID_ID);
            break;
        case 16://subi
            i_type(ID_RR,instr, ID_ID);
            break;
        case 17:// addi
            i_type(ID_RR,instr, ID_ID);
            break;
        case 18://lw
            load_store_instr(ID_RR,instr, ID_ID);
            break;
        case 19://sw
            load_store_instr(ID_RR,instr, ID_ID);
            break;;
        case 20:
            if (ID_RR[PC][0] == 18 || ID_RR[PC][0] == 19) {cycles_in_stall = 3;}
            else if(ID_RR[PC-1][0] == 18 || ID_RR[PC-1][0] == 19) {cycles_in_stall = 2; }
            else {cycles_in_stall = 1; }
            ID_RR[PC+1].push_back(0); //[1] blank
            ID_RR[PC+1].push_back(0); //[2] blank
            ID_RR[PC+1].push_back(0); //[3] blank
            ID_RR[PC+1].push_back(1); //[4] EX control signal
            ID_RR[PC+1].push_back(0); //[5] blank
            ID_RR[PC+1].push_back(0); //[6] blank
            ID_RR[PC+1].push_back(1); //[7] control
            end = true; 
            break;
        default:
            cout<<"no operation found of given kind. check the instructions."<< endl;
            break;
    }
    if(not end) {data_hazards(ID_RR, bypass);}
    else {bypass.push_back("n");}
}

void instr_decode_2::data_hazards(vector<vector<int> >& ID_RR, vector<string> &bypass){
    int prev_to_prev_instr, prev_instr, curr_instr, next_instr;
    vector<int> prev_to_prev_regs, prev_regs, curr_regs, next_regs;
    curr_instr = opr_types(ID_RR[PC][0]);// type of current register(3 for r , 2 for i)
    next_instr = opr_types(ID_RR[PC+1][0]);//type of current register(3 for r , 2 for i)
    for (int i=0; i<curr_instr;i++){
        curr_regs.push_back(ID_RR[PC][i+1]);
    }
    for (int i=0; i<next_instr;i++){
        next_regs.push_back(ID_RR[PC+1][i+1]);
    }
    bool bypass_done = false; 
    if (curr_instr == 3){
        if (next_instr == 3){
            if (curr_regs[0] == next_regs[1] || curr_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
            
        }

        else {
            if (curr_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
            
        }
    }
    else{
        int opcode = ID_RR[PC][0];
        if (next_instr == 2){
        if (opcode == 18){
            if (curr_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 2;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
            }
            else if (ID_RR[PC+1][0] == 19){
            if (curr_regs[0] == next_regs[1] || curr_regs[0] == next_regs[0]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 2;
                }
            }
        }
            
            }
        else if (opcode == 19){
            if (curr_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
            
        }
        else {
            if (curr_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
            
        }
    }
        if (next_instr == 3){
        if (opcode == 18){
            if (curr_regs[0] == next_regs[1] || curr_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 2;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
            }
            
            }
        else if (opcode == 19){
            if (curr_regs[0] == next_regs[1] || curr_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
           
        }
        else {
            if (curr_regs[0] == next_regs[1] || curr_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
            
        }
    }
    }
    if (PC > 0){
        if (cycles_in_stall==0){
            prev_instr = opr_types(ID_RR[PC-1][0]);
            for (int i=0;i<prev_instr;i++){
                prev_regs.push_back(ID_RR[PC-1][i+1]);
            }
            if (curr_instr == 3){
                if (next_instr == 3){
                if (prev_regs[0] == next_regs[1] || prev_regs[0] == next_regs[2]){
                    if (cycles_in_stall==0){
                        cycles_in_stall = 0;
                    }
                    if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                }
                
            }
            else {
                if (prev_regs[0] == next_regs[1]){
                    if (cycles_in_stall==0){
                        cycles_in_stall = 0;
                    }
                    if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                }
                
                }
            }
            else{
                int opcode = ID_RR[PC-1][0];
                if (next_instr == 2){
                if (opcode == 18){
            if (prev_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 1;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
            }
            else if (ID_RR[PC+1][0] == 19){
            if (prev_regs[0] == next_regs[1] || prev_regs[0] == next_regs[0]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 1;
                }
            }
        }
            
            }
        else if (opcode == 19){
            if (prev_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
            
        }
        else {
            if (prev_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
            
        }
    }
        if (next_instr == 3){
        if (opcode == 18){
            if (prev_regs[0] == next_regs[1] || prev_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 1;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
            }
        
            }
        else if (opcode == 19){
            if (prev_regs[0] == next_regs[1] || prev_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
        
        }
        else {
            if (prev_regs[0] == next_regs[1] || prev_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
                if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
            }
        
        }
    }
            }

        }
    }
    if (PC > 1) {
        if (cycles_in_stall==0){
            prev_to_prev_instr = opr_types(ID_RR[PC-2][0]);
            for (int i=0;i<prev_to_prev_instr;i++){
                prev_to_prev_regs.push_back(ID_RR[PC-2][i+1]);
            }
            if (curr_instr == 3){
                if (next_instr == 3){
                if (prev_to_prev_regs[0] == next_regs[1] || prev_to_prev_regs[0] == next_regs[2]){
                    if (cycles_in_stall==0){
                        cycles_in_stall = 0;
                    }
                    if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                }
                
                }
                else {
                    if (prev_to_prev_regs[0] == next_regs[1]){
                        if (cycles_in_stall==0){
                            cycles_in_stall = 0;
                        }
                        if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                    }
                    
                }
            }
            else{
                int opcode = ID_RR[PC-2][0];
                if (next_instr == 2){
                if (opcode == 18){
                    if (prev_to_prev_regs[0] == next_regs[1]){
                        if (cycles_in_stall==0){
                            cycles_in_stall = 0;
                        }
                        if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
                    }
                    
                    }
                else if (opcode == 19){
            if (prev_to_prev_regs[0] == next_regs[1]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
            
        }
                else {
                    if (prev_to_prev_regs[0] == next_regs[1]){
                        if (cycles_in_stall==0){
                            cycles_in_stall = 0;
                        }
                        if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                    }
                    
                }
            }
            else {
                if (opcode == 18){
                    if (prev_to_prev_regs[0] == next_regs[1] || prev_to_prev_regs[0] == next_regs[2]){
                        if (cycles_in_stall==0){
                            cycles_in_stall = 0;
                        }
                        if(not bypass_done) {bypass_done = true; bypass.push_back("b");}
                    }
                    
                    }
                else if (opcode == 19){
            if (prev_to_prev_regs[0] == next_regs[1] || prev_to_prev_regs[0] == next_regs[2]){
                if (cycles_in_stall==0){
                    cycles_in_stall = 0;
                }
            }
            
        }
                else {
                    if (prev_to_prev_regs[0] == next_regs[1] || prev_to_prev_regs[0] == next_regs[2]){
                        if (cycles_in_stall==0){
                            cycles_in_stall = 0;
                        }
                        if(not bypass_done) {bypass_done = true; bypass.push_back("a");}
                    }
                    
                }
            }
        } }
    }
    if (not bypass_done){bypass_done = true ;bypass.push_back("n");}
}

void instr_decode_2::jump_instr(vector<vector<int> >&ID_RR, string instr, vector<vector<string> > &ID_ID){
    string conv;
    vector<string> cmd;
    int pos = stoi(ID_ID[PC][2]);
    conv = instr[pos];
    while (pos != instr.length()){
        if (conv == " "){
            pos++;
            conv = instr[pos];
        }else{
            cmd.push_back(conv);
            pos++;
            conv = instr[pos];
        }
    }
    string label;
    label = vec_to_str(cmd);
    label_list.push_back(label);
    ID_RR[PC+1].push_back((label_list.size())-1);
    cmd.clear();
    ID_RR[PC+1].push_back(0);//buffer
    ID_RR[PC+1].push_back(0);//buffer
    ID_RR[PC+1].push_back(1);//ex
    ID_RR[PC+1].push_back(0);//mem
    ID_RR[PC+1].push_back(0);//wb
    ID_RR[PC+1].push_back(1); //[7] control
}

void instr_decode_2::r_type(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID){
    string conv;
    int pos = stoi(ID_ID[PC][2]);
    conv = instr[pos]; // character at pos in instr
    vector<string> cmd;
    int reg_count = 0; // to read the name of the register 
    //reading the regisster name
    while (pos != instr.length()){
        // if (reg_count == 2){
        //     int constt;
        //     sscanf(&instr[pos],"%d", &constt);
        //     ID_EX[PC+1].push_back(constt);
        //     break;
        // }
        if (conv == " "){
            pos++;
            conv = instr[pos];
        }
        else if (conv != ","){
            cmd.push_back(conv);
            pos++;
            conv = instr[pos];
        }
        else{
            reg_search(cmd,reg_no,reg_count);
            ID_RR[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
            pos++;
            conv = instr[pos];
        }
    }
    reg_search(cmd,reg_no, reg_count);
    ID_RR[PC+1].push_back(1);// execution control
    ID_RR[PC+1].push_back(0);//memory contorl
    ID_RR[PC+1].push_back(1);// write back control
    ID_RR[PC+1].push_back(1); //[7] control
}

void instr_decode_2::i_type(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID){
    string conv;
    vector<string> cmd;
    int reg_count = 0;
    string delim = ",";
    int pos = stoi(ID_ID[PC][2]);
    conv = instr[pos];
    while (pos != instr.length()){
        if (reg_count == 2){
            int constt;
            sscanf(&instr[pos],"%d", &constt);
            ID_RR[PC+1].push_back(constt);
            break;
        }
        if (conv != delim){
            cmd.push_back(conv);
        }
        else{
            reg_search(cmd,reg_no,reg_count);
            ID_RR[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
        }
        pos++;
        conv = instr[pos];
        if (conv == " "){
            pos++;
            conv = instr[pos];
        }
    }
    ID_RR[PC+1].push_back(1);// execution control
    ID_RR[PC+1].push_back(0);// memory contorl
    ID_RR[PC+1].push_back(1);// write back control
    ID_RR[PC+1].push_back(1); //[7] control
}

void instr_decode_2:: load_store_instr(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID){
    string conv;
    vector<string> cmd;
    int pos = stoi(ID_ID[PC][2]);
    conv = instr[pos];
    string offset;
    int reg_count = 0;
    while (reg_count != 2){
        if (conv == "$"){
            cmd.push_back(conv);
            conv = instr[pos+1];
            cmd.push_back(conv);
            conv = instr[pos+2];
            if (conv ==  " " or conv =="," or conv ==")"){
                pos+=0;
            }else{
                cmd.push_back(conv);
                pos+=2;
            }
            reg_search(cmd,reg_no,reg_count);
            ID_RR[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
        }
        else if(conv ==","){
            pos++;
            conv = instr[pos];
            while(conv !="("){
                if (conv==" "){
                    conv = instr[pos+1];
                    pos++;
                }else{
                offset += conv;
                conv = instr[pos+1];
                pos++;
                }
            };
            ID_RR[PC+1].push_back(stoi(offset));
            cmd.clear();
        }
        pos++;
        conv = instr[pos];
    }
    int temp = ID_RR[PC+1][2];
    ID_RR[PC+1][2] =ID_RR[PC+1][3];
    ID_RR[PC+1][3] = temp;
    ID_RR[PC+1].push_back(1); // empty instruciotn
    ID_RR[PC+1].push_back(1); // ex control
    ID_RR[PC+1].push_back(1); // mem control
    ID_RR[PC+1].push_back(1); //[7] control
}

void instr_decode_2:: branch_instr(vector<vector<int> >& ID_RR, string instr, vector<vector<string> > &ID_ID){
    string conv;
    vector<string> cmd;
    int pos = stoi(ID_ID[PC][2]);
    conv = instr[pos];
    int reg_count = 0 ;
    while(reg_count != 3){
        if (reg_count == 2){
            if (conv == " "){
                cmd.clear();
            }else{
                while(pos != instr.length()){
                    cmd.push_back(conv);
                    pos++;
                    conv = instr[pos];
                }
                string label;
                label = vec_to_str(cmd);
                label_list.push_back(label);
                ID_RR[PC+1].push_back((label_list.size())-1);
                cmd.clear();
                reg_count++;
            }
        }else if(conv == "$"){
            cmd.push_back(conv);
            conv = instr[pos+1];
            cmd.push_back(conv);
            conv = instr[pos+2];
            cmd.push_back(conv);
            pos+=3;
            reg_search(cmd,reg_no, reg_count);
            ID_RR[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
        }
        pos++;
        conv = instr[pos];
    }
    ID_RR[PC+1].push_back(1); // control signal for ex
    ID_RR[PC+1].push_back(0);// mem
    ID_RR[PC+1].push_back(0);// wb
    ID_RR[PC+1].push_back(1); //[7] control
}

string instr_decode_2::vec_to_str(vector<string>& vec){
    string r = "";
    for (int i= 0;i<vec.size(); i++){
        r.append(vec[i]);
    }
    return r;
}

void instr_decode_2::reg_search(vector<string>& opr_name, int& r, int reg_count){
    string opr_str = vec_to_str(opr_name);
    char sechar = opr_str[1];
    r=0;
    if (reg_count ==0){
        if(opr_str == "$0" || opr_str == "$zero") {
            cout << "invalid register name, can't write in a constant( $0 or $zero) reg"<<endl;
            exit(0);
        }
    }
    if (isalpha(sechar)){
        while (opr_str != reg_names_1[r]){
            r++;
        }
    }else{
        while (opr_str != reg_names_2[r]){
            r++;
        }
    }
}

int instr_decode_2::opr_types(int opr){
    int res;
    if ((opr == 0 || opr == 1 || opr == 2 || opr == 3 || opr == 4 || opr == 5 || opr== 6 || opr==7 || opr==8 || opr==9 || opr==10 || opr==11 || opr== 12)){
        res = 3;
        return res;
    }
    else if ((opr == 14 || opr == 15 || opr == 16 || opr == 17 || opr == 18 || opr == 19)){
        res = 2;
        return res;
    }else{
        return 0; 
    }
}

// ---------------------------------------- ******************* ---------------------------------------- // 




#endif