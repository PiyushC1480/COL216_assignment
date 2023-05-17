#include <vector>
# include <iostream>

#ifndef _ALU_H
#define _ALU_H
//ALU will only handle r , i and branch instruction . jump lw and sw are not handeled by alu.
using namespace std;

class alu{
    public:
    int r_type(int PC, vector<vector<int> >& EX_MEM,int opr, int rs, int rt);
    int i_type(int PC, vector<vector<int> >& EX_MEM,int opr, int rs, int numeric);
    int branch(int opr, int rs, int rt);
};
//------------------ALU funcitons--------------------
//-------sll and srl    in i type instructon
int alu::r_type(int PC, vector<vector<int> >& EX_MEM,int opr, int rs, int rt){
    int data1, data2, output;
    data1 = rs;
    data2 = rt;
    switch(opr){
        case 0:
            output = data1 + data2;
            break;
        case 1: 
            output = data1 - data2;
            break;
        case 4:
            if (data1<data2){ output = 1;} else {output=0;}
            break;
        case 5:
            output  = data1 & data2;
            break;
        case 6:
            output = data1 | data2 ;
            break;
        case 7:
            output = ~(data1|data2);
            break;
    }
    EX_MEM[PC+1].push_back(output);
    return output;
}

int alu::i_type(int PC, vector<vector<int> >& EX_MEM,int opr, int rs, int numeric){
    int data1, data2, output;
    data1 = rs;
    data2 = numeric;
    switch(opr){
        case 2:
            output  = data1 * (2^(data2));
            break;
        case 3:
            output  = data1 / (2^(data2));
            break;
        case 14:
            output = data1 & data2;
            break;
        case 15:
            output = data1 | data2 ;
            break;
        case 16:
            output = data1 -data2;
            break;
        case 17:
            output  = data1 + data2;
            break;
    }
    EX_MEM[PC+1].push_back(output);
    return output;
}

int alu::branch(int opr, int rs, int rt){
    int output = 0;
    switch(opr){
        case 8:
            if (rs==rt){
                output =1;
            }
            break;
        case 9:
            if (rs != rt){
                output = 1;
            }
            break;
        case 10:
            if (rs> rt) {
                output = 1;
            }
            break;
        case 11:
            if (rs <= rt){
                output = 1;
            }
            break;
        case 12:
            if (rs>= rt){
                output = 1;
            }
            break;
    }
    return output;
}
#endif
