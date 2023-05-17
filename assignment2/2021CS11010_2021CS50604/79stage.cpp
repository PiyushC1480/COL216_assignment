#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./79_stage_no_bypass/pc_7_without.hpp"
#include "./79_stage_no_bypass/if_7_without.hpp"
#include "./79_stage_no_bypass/id_7_without.hpp"
#include "./79_stage_no_bypass/ex_7_without.hpp"
#include "./79_stage_no_bypass/wb_7_without.hpp"
#include "./79_stage_no_bypass/read_7_without.hpp"

bool debug = false; 
bool lw_sw = false;
bool print_cycle = true; 

// ---------------------------------------- HELPER FUNTIONS -------------------------------------------// 

void upd_str(vector<vector<string> >& vect, int PC ){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<string>());
        for (int j=0; j<2;j++){
            vect[0].push_back("0");
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<string>());
            for (int l=0;l<2;l++){
                vect[k].push_back("0");
            }
        }
    }

}

void upd_str1(vector<vector<string> >& vect, int PC ){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<string>());
        for (int j=0; j<3;j++){
            vect[0].push_back("0");
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<string>());
            for (int l=0;l<3;l++){
                vect[k].push_back("0");
            }
        }
    }
}

void upd_str2(vector<vector<string> >& vect, int PC ){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<string>());
        for (int j=0; j<4;j++){
            vect[0].push_back("0");
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<string>());
            for (int l=0;l<4;l++){
                vect[k].push_back("0");
            }
        }
    }
}

void upd_uns(vector<vector<int> > & vect, int PC){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<int>());
        for (int j=0; j<8;j++){
            vect[0].push_back(0);
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<int>());
            for (int l=0;l<8;l++){
                vect[k].push_back(0);
            }
        }
    }
}

void upd_int(vector<vector<int> > & vect, int PC){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<int>());
        for (int j=0; j<7;j++){
            vect[0].push_back(0);
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<int>());
            for (int l=0;l<7;l++){
                vect[k].push_back(0);
            }
        }
    }
}


void upd_int1(vector<vector<int> > & vect, int PC){
    int i = 0;
    for (int j=0; j<vect.size(); j++){
        i++;
    }
    // we could just do i = vect.size();
    if (!PC){
        vect.push_back(vector<int>());
        for (int j=0; j<4;j++){
            vect[0].push_back(0);
        }
    }
    else if (PC>0){
        for(int k=i;k<(PC+1);k++){
            vect.push_back(vector<int>());
            for (int l=0;l<4;l++){
                vect[k].push_back(0);
            }
        }
    }
}

void reset_mem(){
    fstream dm_rm;
    // dm_rm.open("data_mem.txt", fstream::out | fstream :: trunc);
    // dm_rm.close();

    dm_rm.open("reg_7_without.txt", fstream::out | fstream :: trunc);
    for(int i=0; i<32; i++){
        dm_rm << 0 << endl;
    }
    dm_rm.close();
}

void upd_reg(int PC,vector<vector<string> >& IF_ID, vector<vector<string> >& IF_IF, vector<vector<int> >& ID_RR, vector<vector<int> >& RR_ALU, vector<vector<int> >&EX_MEM, vector<vector<int> >& MEM_WB, vector<vector<string> >&ID_ID, vector<vector<int> >&DM_DM){
    upd_str(IF_ID, PC);
    upd_str1(IF_IF, PC);
    upd_str2(ID_ID, PC);
    upd_uns(ID_RR, PC);
    upd_int(RR_ALU, PC);
    upd_int(EX_MEM, PC);
    upd_int(DM_DM, PC);
    upd_int1(MEM_WB, PC);
}

int opr_types(int opr){
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

void hazard_detection(instr_decode_1 &ID1_state, instr_decode_2 &ID2_state, exe &ALU_state, bool &is_data_haz, int &stalls, read_reg &RR_state, instr_fetch_2 &IF2_state){
    if (stalls>0){
        ID1_state.upd_stall(true);
        ID2_state.upd_stall(true);
        ALU_state.upd_stall(true);
        RR_state.upd_stall(true);
        IF2_state.upd_stall(true);
        stalls--;
    }
    if (ID2_state.check_stall_cycle() != 0){
        stalls = ID2_state.check_stall_cycle();
        ID2_state.reset_stall_cycle();
        is_data_haz = true;
        ID2_state.upd_stall(true);
        ID1_state.upd_stall(true);
        ALU_state.upd_stall(true);
        RR_state.upd_stall(true);
        IF2_state.upd_stall(true);
    }
    if(stalls == 0){
        is_data_haz = false;
        ID1_state.upd_stall(false);
        ID2_state.upd_stall(false);
        ALU_state.upd_stall(false);
        RR_state.upd_stall(false);
        IF2_state.upd_stall(false);
    }
}

void reset_reg (int PC,vector<vector<string> >&IF_ID,vector<vector<string> >&IF_IF,vector<vector<string> >&ID_ID,vector<vector<int> >&RR_ALU,vector<vector<int> >&EX_MEM,vector<vector<int> >&MEM_WB, vector<vector<int> > &ID_RR){
    IF_IF.clear();
    IF_ID.clear();
    ID_ID.clear();
    ID_RR.clear();
    RR_ALU.clear();
    EX_MEM.clear();
    MEM_WB.clear();
    upd_str1(IF_IF, PC);
    upd_str2(ID_ID, PC);
    upd_str(IF_ID,PC);
    upd_int(RR_ALU,PC);
    upd_uns(ID_RR,PC);
    upd_int(EX_MEM,PC);
}

// *********************************************  NOTES ************************************************* //

// IF_IF - vector of vector of strings == [i_fetch, control, is_fetched]
// IF_ID - vector of vector of strings == [instruction, control signal]
// ID_ID - vector of vector of strings == [op code, instr, pos, control signal]
// ID_RR - vecotor of vector of int == [op_code, reg1, reg2, reg3/constant, ex control, mem control, wb control, pass control]
// RR_ALU -- vector of vector of int == [op_code, reg1, reg2, reg3/constant, ex control, mem control, wb control, reg2_val, reg3_val/blank]
// EX-MEM -- vector of vector of int == [mem control, wb_control, output, reg address ,0(for non lw-sw)/18(for lw)/19(for sw), dm2_control]
//                                      [mem control, wb_control, reg1/val of reg1, dm address, ls/sw, dm2_control]
// DM_DM -- vector of vector of int == [mem control, wb_control, val, reg 1, dm address, lw/sw, dm2control]
// MEM_WB -- vector of vector of int == []

// ==========================================    MAIN FUNCTION ============================================= //

// 7-9 stage pipeline : 
//  IF   IF   ID   ID   RR   ALU |  WB             --- for non lw-sw instructions
//                           ALU |  DM   DM   WB   --- for lw-sw instructions

int main(){
    int stalls = 0;
    bool is_data_haz = false;   
    pc Prg_Ctr;
    Prg_Ctr.setPC(0);

    //registers for pipeline
    vector<vector<int> > ID_RR;
    vector<vector<int> > RR_ALU, EX_MEM, DM_DM, MEM_WB; 
    vector<string> instr_mem;
    vector<vector<string> > IF_IF, IF_ID, ID_ID; 

    //states initialization
    instr_fetch_1 IF1_state;
    instr_fetch_2 IF2_state; 
    instr_decode_1 ID1_state; 
    instr_decode_2 ID2_state; 
    read_reg RR_state; 
    alu ALU; 
    memory_1 DM1_state; 
    memory_2 DM2_state; 
    wb WB_state; 
    exe ALU_state; 

    upd_reg(0,IF_ID, IF_IF, ID_RR, RR_ALU, EX_MEM, MEM_WB, ID_ID, DM_DM);
    DM1_state.init();
    reset_mem();

    IF1_state.read_instr(instr_mem, Prg_Ctr.getPC());

    while(!ALU_state.is_end()){

        // ======================================== Non LW-SW INSTRUCTIONS ===================================== //
        
        if (EX_MEM[Prg_Ctr.getPC()][4] == 0){
        // ------------------------------ WB STATE --------------------------------- // 
            if (EX_MEM[Prg_Ctr.getPC()][1]){
                WB_state.write_back(EX_MEM[Prg_Ctr.getPC()][2], EX_MEM[Prg_Ctr.getPC()][3]);
                if (debug) cout<< "------------------- WB executed ------------------"<<endl;
            }
        }

        if (EX_MEM[Prg_Ctr.getPC()][4] != 0 || DM_DM[Prg_Ctr.getPC()][5] != 0 || MEM_WB[Prg_Ctr.getPC()][3] == 9) {
        // ======================================== LW-SW INSTRUCTIONS ===================================== //

        // ------------------------------ DM STATE 1 --------------------------------- // 
                                /* For handling lw instructions */
        if(EX_MEM[Prg_Ctr.getPC()][0]){
            DM_DM.push_back(vector<int>());
            DM_DM[Prg_Ctr.getPC()+1].push_back(EX_MEM[Prg_Ctr.getPC()][0]);
            if (EX_MEM[Prg_Ctr.getPC()][4] == 18){
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(1);
                int load_val = DM1_state.read(EX_MEM[Prg_Ctr.getPC()][3],Prg_Ctr.getPC(), DM_DM);
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][2]);
                DM_DM[Prg_Ctr.getPC()+1].push_back(EX_MEM[Prg_Ctr.getPC()][3]);
                DM_DM[Prg_Ctr.getPC()+1].push_back(EX_MEM[Prg_Ctr.getPC()][4]);
                DM_DM[Prg_Ctr.getPC()+1].push_back(1);
                if (debug) cout<< "------------------- DM 1 executed ------------------"<<endl;
            }
            
            else{
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][1]);
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][2]);
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(0); // value to be loaded/stored
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][3]);
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][4]);
                DM_DM[(Prg_Ctr.getPC()+1)].push_back(1);
            }
        }

        // ------------------------------ DM STATE 2 --------------------------------- // 
                                /* For handling sw instructions */
        if(DM_DM[Prg_Ctr.getPC()][6]){
            MEM_WB.push_back(vector<int>());
            if (DM_DM[Prg_Ctr.getPC()][5] == 19){
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(0);
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(0);
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(0);
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(9);  // 9 stage instruction
                DM2_state.store(DM_DM[Prg_Ctr.getPC()][2], DM_DM[Prg_Ctr.getPC()][4]);
                // MEM_state.store(EX_MEM[Prg_Ctr.getPC()][3], val);
                if (debug) cout<< "------------------- DM 2 executed ------------------"<<endl;
            } 
            else{
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(DM_DM[Prg_Ctr.getPC()][1]);// contorl signal for write back
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(DM_DM[Prg_Ctr.getPC()][2]);
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(DM_DM[Prg_Ctr.getPC()][3]);
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(9); // 9 stage instruction  
        }
        }

        // ------------------------------ WB STATE ---------------------------------- // 
        if (MEM_WB[Prg_Ctr.getPC()][0]){
            WB_state.write_back(MEM_WB[Prg_Ctr.getPC()][1], MEM_WB[Prg_Ctr.getPC()][2]); 
        }
    }

        // ---------------------------- EX STATE ----------------------------------- //
        ALU_state.upd_PC(Prg_Ctr.getPC());
        // if(ALU_state.check_stall()){
        //     RR_ALU.push_back(vector<int>());
        //     for (int i=0; i<RR_ALU[Prg_Ctr.getPC()].size();i++){
        //             RR_ALU[Prg_Ctr.getPC()+1].push_back(RR_ALU[Prg_Ctr.getPC()][i]);
        //         }
        //     EX_MEM.push_back(vector<int>());
        //     for (int i=0; i<RR_ALU[Prg_Ctr.getPC()].size();i++){
        //         EX_MEM[Prg_Ctr.getPC()+1].push_back(EX_MEM[Prg_Ctr.getPC()][i]);
        //     }
        // }
        if(RR_ALU[Prg_Ctr.getPC()][4]){
            ALU_state.label_list = ID2_state.label_list;
            ALU_state.ex(Prg_Ctr.getPC(),RR_ALU, EX_MEM, ALU, instr_mem);
            if (debug) cout<< "------------------- ALU executed ------------------"<<endl;
        }

        // ---------------------------- RR STATE ----------------------------------- //
        if(RR_state.check_stall()){
            ID_RR.push_back(vector<int>());
            for (int i=0; i<ID_RR[Prg_Ctr.getPC()].size();i++){
                    ID_RR[Prg_Ctr.getPC()+1].push_back(ID_RR[Prg_Ctr.getPC()][i]);
                }
        }
        else if (ID_RR[Prg_Ctr.getPC()][7] == 1){
            RR_state.fetch(ID_RR, Prg_Ctr.getPC(), RR_ALU);
            if (debug) cout<< "------------------- RR_STATE executed ------------------"<<endl;
        }

        // ---------------------------- ID STATE 2 --------------------------------- //
        if (ID2_state.check_stall()){
            ID_ID.push_back(vector<string>());
            ID_ID[Prg_Ctr.getPC()+1].push_back(ID_ID[Prg_Ctr.getPC()][0]);
            ID_ID[Prg_Ctr.getPC()+1].push_back(ID_ID[Prg_Ctr.getPC()][1]);
            ID_ID[Prg_Ctr.getPC()+1].push_back(ID_ID[Prg_Ctr.getPC()][2]);
            ID_ID[Prg_Ctr.getPC()+1].push_back(ID_ID[Prg_Ctr.getPC()][3]);
        }
        else if (stoi(ID_ID[Prg_Ctr.getPC()][3])){
            ID2_state.OP_code(ID_ID, Prg_Ctr.getPC(), ID_RR);
            if (debug) cout<< "------------------- ID_STATE 2 executed ------------------"<<endl;
        }

        // --------------------------- ID STATE 1 ---------------------------------- //
        if (ID1_state.check_stall()){
            IF_ID.push_back(vector<string>());
            IF_ID[Prg_Ctr.getPC()+1].push_back(IF_ID[Prg_Ctr.getPC()][0]);
            IF_ID[Prg_Ctr.getPC()+1].push_back(IF_ID[Prg_Ctr.getPC()][1]);
        }
        else if(stoi(IF_ID[Prg_Ctr.getPC()][1])){
            ID1_state.decoder(IF_ID, ID_ID, Prg_Ctr.getPC());
            if (debug) cout<< "------------------- ID_STATE 1 executed ------------------"<<endl;
        }

        // --------------------------- IF STATE 2 ---------------------------------- //  
        
        if (IF2_state.check_stall()){
            IF_IF.push_back(vector<string>());
            IF_IF[Prg_Ctr.getPC()+1].push_back(IF_IF[Prg_Ctr.getPC()][0]);
            IF_IF[Prg_Ctr.getPC()+1].push_back(IF_IF[Prg_Ctr.getPC()][1]);
            IF_IF[Prg_Ctr.getPC()+1].push_back(IF_IF[Prg_Ctr.getPC()][2]);
        }
        else if (IF_IF[Prg_Ctr.getPC()][1] == "1"){
            IF2_state.next_instr(IF_IF, instr_mem, Prg_Ctr.getPC(), IF_ID); 
            if (debug) cout<< "------------------- IF_STATE 2 executed ------------------"<<endl; 
            if (debug) cout << "------------------ Instructions fetched :"<<IF_IF[Prg_Ctr.getPC()][0]<<endl;
        }
         
        // --------------------------- IF STATE 1----------------------------------- //
        if (!is_data_haz){
            IF1_state.incr_i_fetch(Prg_Ctr.getPC(), IF_IF);
            if (debug) cout<< "------------------- IF_STATE 1 executed ------------------"<<endl;
        }



    // ======================================== LW-SW INSTRUCTIONS ===================================== //

        if (ALU_state.return_PC() != Prg_Ctr.getPC()){
            Prg_Ctr.setJumpTo(ALU_state.return_PC());
            Prg_Ctr.findWhenToJump();
            IF1_state.jump_to(true,0);
            ID2_state.set_jump(true);
            ALU_state.set_jump(true);
            Prg_Ctr.inc();
            hazard_detection(ID1_state, ID2_state, ALU_state, is_data_haz, stalls, RR_state, IF2_state);
            upd_reg(Prg_Ctr.getPC(),IF_ID, IF_IF, ID_RR, RR_ALU, EX_MEM, MEM_WB, ID_ID, DM_DM);
        }
        else if(Prg_Ctr.getPC() == Prg_Ctr.whenToJump()){
            Prg_Ctr.setPC(Prg_Ctr.jumpTo());
            Prg_Ctr.setWhenToJump(-1);
            IF1_state.jump_to(false, Prg_Ctr.getPC());
            ID2_state.set_jump(false);
            reset_reg(Prg_Ctr.getPC(),IF_ID, IF_IF, ID_ID, RR_ALU, EX_MEM, MEM_WB, ID_RR);
        }
        
        else{
            Prg_Ctr.inc();
            if (debug) cout<<"----------------------- CYCLE "<<Prg_Ctr.getPC()<<"-----------------------------"<<endl;
            hazard_detection(ID1_state, ID2_state, ALU_state, is_data_haz, stalls, RR_state, IF2_state);
            upd_reg(Prg_Ctr.getPC(),IF_ID, IF_IF, ID_RR, RR_ALU, EX_MEM, MEM_WB, ID_ID, DM_DM);
        }
        if (!ALU_state.is_end()){
        ifstream reg;
        string line; 
        reg.open("reg_7_without.txt");
        int k = 0; 
        while (getline(reg, line))
        {
            cout<<line<<" ";
        }
        cout<<endl;
        if (DM_DM[Prg_Ctr.getPC()-1][6] & DM_DM[Prg_Ctr.getPC()-1][5] == 19)
        {
            cout << 1<<" "<<DM_DM[Prg_Ctr.getPC()-1][4]<<" "<<DM_DM[Prg_Ctr.getPC()-1][2];
        }
        else{
            cout<<0;
        }
        cout<<endl;}
    }
    return 0;

}

