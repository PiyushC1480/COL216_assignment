#include <iostream>
#include <fstream> 
using namespace std; 

class memory{
    public: 
        int load(int reg_addr, int PC, vector<vector<int> >& MEM_WB); 
        void store(int reg_addr, int value);

        int read(int mem_addr, int PC, vector<vector<int> >& MEM_WB);
        void write(int value, int offset, int PC);
        void init();   // to initialize data memory
};

int memory::load(int reg_addr, int PC, vector<vector<int> >& MEM_WB){
    ifstream reg_file;
    ifstream mem_file;
    string val; 
    reg_file.open("reg_5_without.txt");
    mem_file.open("data_mem_5_without.txt");
    int line_num = 0;
    while(getline(reg_file, val)){
        if(line_num == reg_addr){
            int value = stoi(val);
            MEM_WB[PC+1].push_back(value);
            return value;
        }
        else line_num++;
    }
    reg_file.close();
};

void memory::store(int reg_addr, int value){
    string line;
    fstream reg_file;
    reg_file.open("data_mem_5_without.txt");
    vector<string> temporary;
    while(getline(reg_file, line)){
        temporary.push_back(line);
    }
    reg_file.close();
    ofstream updated_mem;
    updated_mem.open("data_mem_5_without.txt");
    for(int line_no=0; line_no<temporary.size(); line_no++){
        if (line_no == value){
            updated_mem<<reg_addr<<endl;
        }
        else{
            updated_mem<<temporary[line_no]<<endl;
        }
    }
    updated_mem.close();
    /*
    temporary[reg_addr] = value;
    File.open("reg.txt",fstream::out|fstream::trunc)
    for(int i=0; i< temporary.size();i++){
        reg_file << temporary[i] <,endl;
    }
    reg_file.close();
    */
};

int memory::read(int mem_addr, int PC, vector<vector<int> >& MEM_WB){
    ifstream mem_file;
    string line;
    int val;
    int i =0 ;
    mem_file.open("data_mem_5_without.txt");
    while(getline(mem_file,line)){
        if(i==mem_addr){
            val  =stoi(line);
            mem_file.close();
            MEM_WB[PC+1].push_back(val);
            return val;
        }else{
            i++;
        }
    }
}

void memory::write(int value, int offset,  int PC){
    string line;
    fstream mem_file;
    mem_file.open("data_mem_5_without.txt");
    vector<int> temp;
    while (getline(mem_file,line)){
        temp.push_back(stoi(line));
    }
    mem_file.close();
    temp.push_back(value);
    mem_file.open("data_mem_5_without.txt",fstream::out| fstream::trunc);
    for(int i = 0; i<temp.size();i++){
        mem_file << temp[i] <<endl;
    }
    mem_file.close();
}

void memory::init(){
    ofstream reg_file;
    reg_file.open("data_mem_5_without.txt");
    for(int i = 0; i <1023; i++){
        reg_file<<0<<endl;
    }
    reg_file<<0;
    reg_file.close();
};