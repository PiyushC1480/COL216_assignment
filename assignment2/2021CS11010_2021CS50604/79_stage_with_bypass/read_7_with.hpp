#include <vector>
#include <algorithm>
#include<iostream>
#include<fstream>

#ifndef _READ_H
#define _READ_H

using namespace std;

class read_reg{
    public : 
        void fetch(vector<vector<int> > &ID_RR, int PC, vector<vector<int> > &RR_ALU);
        int opr_types(int opr);
        void upd_stall(bool update){is_stall = update;}
        bool check_stall(){return is_stall;}
    private: 
        bool is_stall = false;
};

// ========================================== FUNCTION DECLARATIONS ====================================== //

void read_reg::fetch(vector<vector<int> > &ID_RR, int PC, vector<vector<int> > &RR_ALU){
    int op = ID_RR[PC][0];
    RR_ALU.push_back(vector<int>());
    for(int i = 0; i < 7; i++){
        RR_ALU[PC+1].push_back(ID_RR[PC][i]);
    }
        int val;
        ifstream file;
        string line;
        file.open("reg_7_with.txt");
        int address = ID_RR[PC][2];
        int i=0;
        while(getline(file,line)){
            if (i==address){
                val = stoi(line);
                RR_ALU[PC+1].push_back(val);
            }
            i++;
        }
        file.close();

    if (opr_types(op) == 3){
        int val;
        ifstream file;
        string line;
        file.open("reg_7_with.txt");
        int address = ID_RR[PC][3];
        int i=0;
        while(getline(file,line)){
            if (i==address){
                val = stoi(line);
                RR_ALU[PC+1].push_back(val);
            }
            i++;
        }
        file.close();
    }
    if (opr_types(op) == 2){
        int val;
        ifstream file;
        string line;
        file.open("reg_7_with.txt");
        int address = ID_RR[PC][1];
        int i=0;
        while(getline(file,line)){
            if (i==address){
                val = stoi(line);
                RR_ALU[PC+1].push_back(val);
            }
            i++;
            }
        }
        file.close();
};

int read_reg::opr_types(int opr){
    int res;
    if ((opr == 0 || opr == 1 || opr == 2 || opr == 3 || opr == 4 || opr == 5 || opr== 6 || opr==7 || opr==8 || opr==9 || opr==10 || opr==11 || opr== 12)){
        res = 3;
        return res;
    }
    else if ((opr == 14 || opr == 15 || opr == 16 || opr == 17 || opr == 18 || opr == 19)){
        res = 2;
        return res;
    }else{
        return 0 ; 
    }
}

#endif