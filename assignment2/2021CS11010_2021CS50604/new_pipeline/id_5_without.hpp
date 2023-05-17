
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

#ifndef _ID_H
#define _ID_H

using namespace std;

class instr_decode{
    public:
    void decoder(vector<vector<string> >&IF_ID, vector<vector<int> >&ID_EX,int PC, vector<string> branches_not_taken_list);
    int opr_types(int opr);
    void data_hazards(vector<vector<int> >& ID_EX);
    void code_search(vector<string>& opr_name,int& r);//since ID_EX is taken to be vector of vector if int hence all the operations written ins tring will be converted into their numberic(position in the opr_list) and then stored in ID_EX;
    void jump_instr(vector<vector<int> >&ID_EX, string instr);
    void branch_instr(vector<vector<int> >& ID_EX, string instr);
    void load_store_instr(vector<vector<int> >& ID_EX, string instr);
    void i_type(vector<vector<int> >&ID_EX, string instr);
    void r_type(vector<vector<int> >& ID_EX, string instr);
    void reg_search(vector<string>& opr_name, int& r, int reg_count);
    void upd_PC(int new_PC){PC = new_PC;}
    vector<string> label_list;
    void upd_stall(bool update){is_stall = update;}
	bool check_stall(){return is_stall;}
	void set_jump(bool set){is_jump = set;}
	int check_stall_cycle(){return cycles_in_stall;}
	void reset_stall_cycle(){cycles_in_stall = 0;}

    private:
    bool is_jump = false;

    string vec_to_str(vector<string>& vec);
    string operations[24] = {"add","sub","sll","srl","slt","and","or","nor","beq","bne","bgt","ble","bge","j","andi","ori","subi","addi","lw","sw", "end", "blt", "mul","mult"};
    string reg_names_1[32] =  {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
    string reg_names_2[32] =  {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31"};
    int pos; // processing the string ine by one by reading the elements in it;
    int reg_no , code_no, PC;
    bool is_stall = false;
    int cycles_in_stall = 0;
};

void instr_decode::decoder(vector<vector<string> >&IF_ID, vector<vector<int> >&ID_EX,int PC, vector<string> branches_not_taken_list){
    upd_PC(PC);
    //processing the current instruction
    if (is_jump){cout<<"jump branch taken"<<endl;}
    else{
        string instr= IF_ID[PC][0];//fetch he instruction to be decoded
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
                auto iter = find(branches_not_taken_list.begin(), branches_not_taken_list.end(), compact);
                if (iter != branches_not_taken_list.end()){
                    ID_EX[PC+1] = vector<int>(7);
                    branches_not_taken_list.erase(iter);
                    return;
                }else{
                cmd.clear();
                pos+=2;
                }
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
        ID_EX.push_back(vector<int>());
        ID_EX[PC+1].push_back(op);
        switch(op){
            case 0: //add
                r_type(ID_EX,instr);
                break;
            case 1: // sub
                r_type(ID_EX,instr);
                break;
            case 2: //sll
                i_type(ID_EX,instr);
                break;
            case 3: //srl 
                i_type(ID_EX,instr);
                break;
            case 4://slt
                r_type(ID_EX,instr);
                break;
            case 5://and
                r_type(ID_EX,instr);
                break;
            case 6://or
                r_type(ID_EX,instr);
                break;
            case 7://nor
                r_type(ID_EX,instr);
                break;
            case 8://beq
                branch_instr(ID_EX,instr);
                break;
            case 9://bne
                branch_instr(ID_EX,instr);
                break;
            case 10://bgt
                branch_instr(ID_EX,instr);
                break;
            case 11://ble
                branch_instr(ID_EX,instr);
                break;
            case 12://bge
                branch_instr(ID_EX,instr);
                break;
            case 13://j
                //cycles_in_stall=1;
                jump_instr(ID_EX,instr);
                break;
            case 14://andi
                i_type(ID_EX,instr);
                break;
            case 15://ori
                i_type(ID_EX,instr);
                break;
            case 16://subi
                i_type(ID_EX,instr);
                break;
            case 17:// addi
                i_type(ID_EX,instr);
                break;
            case 18://lw
                load_store_instr(ID_EX,instr);
                break;
            case 19://sw
                load_store_instr(ID_EX,instr);
                break;
            case 20:
                cycles_in_stall = 2;
                ID_EX[PC+1].push_back(0); //[1] blank
                ID_EX[PC+1].push_back(0); //[2] blank
                ID_EX[PC+1].push_back(0); //[3] blank
                ID_EX[PC+1].push_back(1); //[4] EX control signal
                ID_EX[PC+1].push_back(0); //[5] blank
                ID_EX[PC+1].push_back(0); //[6] blank
			    break;
            case 21://ble
                branch_instr(ID_EX,instr);
                break;
            case 22://mul
                r_type(ID_EX,instr);
                break;
            case 23://mult
                i_type(ID_EX,instr);
                break;
            default:
                cout<<"no operation found of given kind. check the instructions."<< endl;
                break;
        }
        data_hazards(ID_EX);
    }
}

void instr_decode::data_hazards(vector<vector<int> >& ID_EX){
    int prev_instr, curr_instr, next_instr;
    curr_instr = opr_types(ID_EX[PC][0]);// type of current register(3 for r , 2 for i)
    next_instr = opr_types(ID_EX[PC+1][0]);//type of current register(3 for r , 2 for i)
    
    if (PC ==1){
        if (next_instr == 3){
            if (ID_EX[PC+1][2] == ID_EX[PC][1] != 0 ){
                if(cycles_in_stall <=1){
                    cycles_in_stall=2;
                }
            }else{
                ID_EX[PC+1][2] = ID_EX[PC+1][2];
            }

            if (ID_EX[PC+1][3] == ID_EX[PC][1] != 0){
                if(cycles_in_stall <=1){
                    cycles_in_stall=2;
                }
            }else{
                ID_EX[PC+1][3] = ID_EX[PC+1][3];
            }
        }

        if (next_instr == 2){
            if (ID_EX[PC+1][2] == ID_EX[PC][1] != 0){
                if(cycles_in_stall <=1){
                    cycles_in_stall=2;
                }
            }else{
                ID_EX[PC+1][2] = ID_EX[PC+1][2];
            }
        }
        if (curr_instr ==3){
            ID_EX[PC][2] = ID_EX[PC][2];
            ID_EX[PC][3] = ID_EX[PC][3];
        }else{
            ID_EX[PC][2] = ID_EX[PC][2];
        }
    }
    if (PC >=2){
        prev_instr = opr_types(ID_EX[PC-1][0]);
        if (next_instr ==3){
            if (ID_EX[PC-1][0] == 18){
                if(ID_EX[PC+1][2] == ID_EX[PC-1][2] != 0){
                    if(cycles_in_stall ==0 ){
                        cycles_in_stall=1;
                    }
                }if (ID_EX[PC+1][2] == ID_EX[PC][2] != 0){
                    if(cycles_in_stall <=1){
                        cycles_in_stall=2;
                    }
                }else{
                    ID_EX[PC+1][2] = ID_EX[PC+1][2];
                }

                if(ID_EX[PC+1][3] == ID_EX[PC-1][2] != 0) {
                    if(cycles_in_stall ==0){
                        cycles_in_stall=1;
                    }
                }if (ID_EX[PC+1][3] == ID_EX[PC][2] != 0){
                    if(cycles_in_stall <=1){
                        cycles_in_stall=2;
                    }
                }else{
                    ID_EX[PC+1][3] = ID_EX[PC+1][3];
                }
            }else{
                if (ID_EX[PC][0] == 18){
                    if (ID_EX[PC+1][2] == ID_EX[PC][2] != 0){
                        if(cycles_in_stall <=1){
                            cycles_in_stall=2;
                        }
                    }else{
                        ID_EX[PC+1][2] = ID_EX[PC+1][2];
                    }

                    if (ID_EX[PC+1][3] == ID_EX[PC][2] != 0){
                        if(cycles_in_stall <=1){
                            cycles_in_stall=2;
                        }
                    }else{
                        ID_EX[PC+1][3] = ID_EX[PC+1][3];
                    }
                }else {
                if(ID_EX[PC+1][2] == ID_EX[PC-1][1] != 0){
                    if(cycles_in_stall ==0){
                        cycles_in_stall=1;
                    }
                }
                if (ID_EX[PC+1][2] == ID_EX[PC][1] != 0){
                    if(cycles_in_stall <=1){
                        cycles_in_stall=2;
                    }
                }else{
                    ID_EX[PC+1][2] = ID_EX[PC+1][2];
                }

                if(ID_EX[PC+1][3] == ID_EX[PC-1][1] != 0) {
                    if(cycles_in_stall ==0){
                        cycles_in_stall=1;
                    }
                }
                if (ID_EX[PC+1][3] == ID_EX[PC][1] != 0){
                    if(cycles_in_stall <=1){
                        cycles_in_stall=2;
                    }
                }else{
                    ID_EX[PC+1][3] = ID_EX[PC+1][3];
                }
            }
                }
        }else{
            if (cycles_in_stall==0){
                if (ID_EX[PC+1][2] == ID_EX[PC][1] != 0){
                    if(cycles_in_stall ==0){
                    cycles_in_stall=2;
                    }
                }if(ID_EX[PC+1][2] == ID_EX[PC-1][1] != 0){
                    if(cycles_in_stall ==0){
                        cycles_in_stall=1;
                    }
                }else{
                    ID_EX[PC+1][2] = ID_EX[PC+1][2];
                }
            }
        }
    }
}

void instr_decode::jump_instr(vector<vector<int> >&ID_EX, string instr){
    string conv;
    vector<string> cmd;
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
    ID_EX[PC+1].push_back((label_list.size())-1);
    cmd.clear();
    ID_EX[PC+1].push_back(0);//buffer
    ID_EX[PC+1].push_back(0);//buffer
    ID_EX[PC+1].push_back(1);//ex
    ID_EX[PC+1].push_back(0);//mem
    ID_EX[PC+1].push_back(0);//wb
    cout<< "-------jump instruction decoded ----------" <<endl;
}

void instr_decode::r_type(vector<vector<int> >& ID_EX, string instr){
    string conv;
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
            ID_EX[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
            pos++;
            conv = instr[pos];
        }
    }
    reg_search(cmd,reg_no, reg_count);
    ID_EX[PC+1].push_back(reg_no);
    ID_EX[PC+1].push_back(1);// execution control
    ID_EX[PC+1].push_back(0);//memory contorl
    ID_EX[PC+1].push_back(1);// write back control
}

void instr_decode::i_type(vector<vector<int> >& ID_EX, string instr){
    string conv;
    vector<string> cmd;
    int reg_count = 0;
    string delim = ",";
    conv = instr[pos];
    while (pos != instr.length()){
        if (reg_count == 2){
            int constt;
            sscanf(&instr[pos],"%d", &constt);
            ID_EX[PC+1].push_back(constt);
            break;
        }
        if (conv != delim){
            cmd.push_back(conv);
        }
        else{
            reg_search(cmd,reg_no,reg_count);
            ID_EX[PC+1].push_back(reg_no);
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
    ID_EX[PC+1].push_back(1);// execution control
    ID_EX[PC+1].push_back(0);// memory contorl
    ID_EX[PC+1].push_back(1);// write back control
}

void instr_decode:: load_store_instr(vector<vector<int> >& ID_EX, string instr){
    string conv;
    vector<string> cmd;
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
            ID_EX[PC+1].push_back(reg_no);
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
            ID_EX[PC+1].push_back(stoi(offset));
            cmd.clear();
        }
        pos++;
        conv = instr[pos];
    }
    int temp = ID_EX[PC+1][1];
    ID_EX[PC+1][1] =ID_EX[PC+1][2];
    ID_EX[PC+1][2] = temp;
    ID_EX[PC+1].push_back(1); // empty instruciotn
    ID_EX[PC+1].push_back(1); // ex control
    ID_EX[PC+1].push_back(1); // mem control
}

void instr_decode:: branch_instr(vector<vector<int> >& ID_EX, string instr){
    string conv;
    vector<string> cmd;
    conv = instr[pos];
    int reg_count = 0 ;
    while(reg_count != 3){
        if (reg_count == 2){
            if (conv == " " || conv == ","){
                cmd.clear();
            }else{
                while(pos != instr.length()){
                    if (conv ==" "){
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
                ID_EX[PC+1].push_back((label_list.size())-1);
                cmd.clear();
                reg_count++;
            }
        }else if(conv == "$"){
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
            ID_EX[PC+1].push_back(reg_no);
            cmd.clear();
            reg_count++;
        }
        pos++;
        conv = instr[pos];
    }
    int temp = ID_EX[PC+1][1];
    ID_EX[PC+1][1] = ID_EX[PC+1][3];
    ID_EX[PC+1][3] = temp;
    temp = ID_EX[PC+1][2];
    ID_EX[PC+1][2] = ID_EX[PC+1][3];
    ID_EX[PC+1][3] = temp;
    ID_EX[PC+1].push_back(1); // control signal for ex
    ID_EX[PC+1].push_back(0);// mem
    ID_EX[PC+1].push_back(0);// wb
}

string instr_decode::vec_to_str(vector<string>& vec){
    string r = "";
    for (int i= 0;i<vec.size(); i++){
        r.append(vec[i]);
    }
    return r;
}

void instr_decode::code_search(vector<string>& opr_name,int& r){
    string opr_str = vec_to_str(opr_name);
    r = 0;
    while (opr_str != operations[r]){
        r++;
    }
}

void instr_decode::reg_search(vector<string>& opr_name, int& r, int reg_count){
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

int instr_decode::opr_types(int opr){
    int res;
    if ((opr == 0 || opr == 1 || opr == 2 || opr == 3 || opr == 4 || opr == 5 || opr== 6 || opr==7 || opr==8 || opr==9 || opr==10 || opr==11 || opr== 12 || opr ==21 || opr ==22 || opr == 18 || opr == 19)){
        res = 3;
        return res;
    }
    else if (opr == 14 || opr == 15 || opr == 16 || opr == 17  || opr==23){
        res = 2;
        return res;
    }
}

#endif