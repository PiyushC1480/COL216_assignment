#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "./new_pipeline/pc_5_without.hpp"
#include "./new_pipeline/if_5_without.hpp"
#include "./new_pipeline/id_5_without.hpp"
#include "./new_pipeline/ex_5_without.hpp"
#include "./new_pipeline/wb_5_without.hpp"
void upd_uns(vector<vector<int> > & vect, int PC){
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
void upd_reg(int PC,vector<vector<string> >& IF_ID, vector<vector<int> >& ID_EX, vector<vector<int> >&EX_MEM, vector<vector<int> >& MEM_WB){
    upd_str(IF_ID, PC);
    upd_uns(ID_EX, PC);
    upd_uns(EX_MEM, PC);
    upd_uns(MEM_WB, PC);
}

void detect_hazards(instr_decode& ID_state, exe& EX_state, bool& is_data_haz, int& stalls){
    if (stalls>0){
        ID_state.upd_stall(true);
        EX_state.upd_stall(true);
        stalls--;
    }
    if (ID_state.check_stall_cycle() != 0){
        stalls = ID_state.check_stall_cycle();
        ID_state.reset_stall_cycle();
        is_data_haz = true;
        ID_state.upd_stall(true);
        EX_state.upd_stall(true);
    }
    if(stalls == 0){
        is_data_haz = false;
        ID_state.upd_stall(false);
        EX_state.upd_stall(false);
    }
}

void reset_mem(){
    fstream dm_rm;
    // dm_rm.open("data_mem.txt", fstream::out | fstream :: trunc);
    // dm_rm.close();

    dm_rm.open("reg_5_without.txt", fstream::out | fstream :: trunc);
    for(int i=0; i<32; i++){
        dm_rm << 0 << endl;
    }
    dm_rm.close();
}

void reset_reg (int PC,vector<vector<string> >&IF_ID,vector<vector<int> >&ID_EX,vector<vector<int> >&EX_MEM,vector<vector<int> >&MEM_WB){
    IF_ID.clear();
    ID_EX.clear();
    EX_MEM.clear();
    MEM_WB.clear();
    upd_str(IF_ID,PC);
    upd_uns(ID_EX,PC);
    upd_uns(EX_MEM,PC);
    upd_uns(MEM_WB,PC);
}

int main(int argc, char *argv[]){
    int cycles = 0;
    int stalls = 0;
    bool is_data_haz = false;

    //registers for pipeline
    vector<vector<int> > ID_EX, EX_MEM, MEM_WB, Data_Mem;
    vector<string> instr_mem;
    vector<vector<string> > IF_ID;

    //states initialization
    pc Prg_Ctr;
    instr_fetch IF_state;
    instr_decode ID_state;
    alu ALU;
    exe EX_state;
    memory MEM_state;
    wb WB_state;

    upd_reg(0,IF_ID, ID_EX, EX_MEM, MEM_WB);
    reset_mem();
    MEM_state.init();
    ifstream arg(argv[1]);
    IF_state.read_instr(instr_mem, Prg_Ctr.getPC());

    while (!EX_state.is_end()){
        //-------WB state--------------
        
        //---------------IF state--------------------
        
        //----------------MEM state---------------
        if(EX_MEM[Prg_Ctr.getPC()][0]){
            MEM_WB.push_back(vector<int>());
            //load word instruction
            if (EX_MEM[Prg_Ctr.getPC()][4] == 18){
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(1);
                
                int load_val = MEM_state.read(EX_MEM[Prg_Ctr.getPC()][3],Prg_Ctr.getPC(), MEM_WB);

                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][2]);
                // MEM_WB[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][5]);
            }
            if (EX_MEM[Prg_Ctr.getPC()][4] == 19){
                MEM_WB[(Prg_Ctr.getPC()+1)].push_back(0);
                MEM_state.store(EX_MEM[Prg_Ctr.getPC()][2], EX_MEM[Prg_Ctr.getPC()][3]);
                // MEM_state.store(EX_MEM[Prg_Ctr.getPC()][3], val);
            }
        }
        else if (!EX_MEM[Prg_Ctr.getPC()][0]){
            MEM_WB.push_back(vector<int>());
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][1]);// contorl signal for write back
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][2]);
            MEM_WB[(Prg_Ctr.getPC()+1)].push_back(EX_MEM[Prg_Ctr.getPC()][3]);   
        }

        //---------------ID_state--------------------
        if (ID_state.check_stall()){
            IF_ID.push_back(vector<string>());
            IF_ID[Prg_Ctr.getPC()+1].push_back(IF_ID[Prg_Ctr.getPC()][0]);
            IF_ID[Prg_Ctr.getPC()+1].push_back(IF_ID[Prg_Ctr.getPC()][1]);
        }else if(stoi(IF_ID[Prg_Ctr.getPC()][1])){
            ID_state.decoder(IF_ID, ID_EX, Prg_Ctr.getPC(), EX_state.branches_not_taken_list);
        }
        //------------EX state-----------------------
        EX_state.upd_PC(Prg_Ctr.getPC());
        if(EX_state.check_stall()){
            ID_EX.push_back(vector<int>());
            for (int i=0; i<ID_EX[Prg_Ctr.getPC()].size();i++){
                ID_EX[Prg_Ctr.getPC()+1].push_back(ID_EX[Prg_Ctr.getPC()][i]);
            }
            EX_MEM.push_back(vector<int>());
            for (int i=0; i<EX_MEM[Prg_Ctr.getPC()].size();i++){
                EX_MEM[Prg_Ctr.getPC()+1].push_back(EX_MEM[Prg_Ctr.getPC()][i]);
            }
        }else if(ID_EX[Prg_Ctr.getPC()][4]){
            EX_state.label_list = ID_state.label_list;
            EX_state.ex(Prg_Ctr.getPC(),ID_EX, EX_MEM, ALU, MEM_state,instr_mem);
            if (EX_state.is_end()){
                break;
            }
        }
        if (is_data_haz == false){
            if (EX_state.return_PC() != Prg_Ctr.getPC()){
                Prg_Ctr.setPC(EX_state.return_PC());
                IF_state.i_upd(EX_state.return_PC());
                vector<int> ex_mem_last = EX_MEM.back();
                vector<int> mem_wb_last = MEM_WB.back();
                reset_reg(Prg_Ctr.getPC(),IF_ID,ID_EX,EX_MEM,MEM_WB);
                EX_MEM.push_back(ex_mem_last);
                MEM_WB.push_back(mem_wb_last);
                ID_EX.push_back(vector<int>(7));
                ID_state.reset_stall_cycle();
                stalls=0;
                ID_state.label_list.clear();
                IF_state.next_instr(IF_ID,instr_mem,EX_state.return_PC());
            }else{
                IF_state.next_instr(IF_ID,instr_mem,Prg_Ctr.getPC());
            }
        }
        
        //if there is ajump instruction then the following is elecuted
        
        if (MEM_WB[Prg_Ctr.getPC()][0]){
            WB_state.write_back(MEM_WB[Prg_Ctr.getPC()][1], MEM_WB[Prg_Ctr.getPC()][2]);
        }

        if (!EX_state.is_end()){
        ifstream reg;
        string line; 
        reg.open("reg_5_without.txt");
        int k = 0; 
        while (getline(reg, line))
        {
            cout<<line<<" ";
        }
        cout<<endl;
        if (EX_MEM[Prg_Ctr.getPC()][4]==19)
        {
            cout << 1<<" "<<EX_MEM[Prg_Ctr.getPC()][3]<<" "<< EX_MEM[Prg_Ctr.getPC()][2];
        }
        else{
            cout<<0;
        }
        cout<<endl;
        }
        Prg_Ctr.inc();
        detect_hazards(ID_state,EX_state, is_data_haz, stalls);
        upd_reg(Prg_Ctr.getPC(),IF_ID, ID_EX, EX_MEM, MEM_WB);
        cycles++;
    }
    if(EX_state.is_end()){
    }
    return 0;    
}

