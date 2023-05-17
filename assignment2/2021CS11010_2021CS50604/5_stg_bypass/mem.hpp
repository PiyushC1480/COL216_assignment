#include <iostream>
#include <fstream> 
using namespace std; 

class memory{
    public: 
        void store(int reg_addr, int value, vector<vector<int> >& ID_EX );

        void read(int mem_addr, int PC, vector<vector<int> >& MEM_WB, vector<vector<int> >& ID_EX);
        void init();   // to initialize data memory
};


void memory::store(int reg_addr, int value, vector<vector<int> >& ID_EX){
    string line;
    fstream reg_file;
    reg_file.open("data_mem.txt");
    vector<string> temporary;
    while(getline(reg_file, line)){
        temporary.push_back(line);
    }
    reg_file.close();
    ofstream updated_mem;
    updated_mem.open("data_mem.txt");
    for(int line_no=0; line_no<temporary.size(); line_no++){
        if (line_no == value){
            updated_mem<<reg_addr<<endl;
        }
        else{
            updated_mem<<temporary[line_no]<<endl;
        }
    }
    updated_mem.close();
};

void memory::read(int mem_addr, int PC, vector<vector<int> >& MEM_WB, vector<vector<int> >& ID_EX){
    ifstream mem_file;
    string line;
    int val;
    int i =0 ;
    mem_file.open("data_mem.txt");
    while(getline(mem_file,line)){
        if(i==mem_addr){
            val  =stoi(line);
            mem_file.close();
            MEM_WB[PC+1].push_back(val);
        }else{
            i++;
        }
    }

}

void memory::init(){
    ofstream reg_file;
    reg_file.open("data_mem.txt");
    for(int i = 0; i <1023; i++){
        reg_file<<0<<endl;
    }
    reg_file<<0;
    reg_file.close();
};