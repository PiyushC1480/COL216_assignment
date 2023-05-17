#include <iostream>
#include <fstream> 
using namespace std; 

class memory_1{
    public: 
        int load(int reg_addr, int PC, vector<vector<int> >& DM_DM); 
        int read(int mem_addr, int PC, vector<vector<int> >& DM_DM);
        void init();   // to initialize data memory
};

// ==================================== FUNCTION DECLARATIONS ======================================= //

int memory_1::load(int reg_addr, int PC, vector<vector<int> >& DM_DM){
    ifstream reg_file;
    ifstream mem_file;
    string val; 
    reg_file.open("reg_7_with.txt");
    mem_file.open("data_mem_7_with.txt");
    int line_num = 0;
    while(getline(reg_file, val)){
        if(line_num == reg_addr){
            int value = stoi(val);
            DM_DM[PC+1].push_back(value);
            return value;
        }
        else line_num++;
    }
    reg_file.close();
    return 0 ; 
};

int memory_1::read(int mem_addr, int PC, vector<vector<int> >& DM_DM){
    ifstream mem_file;
    string line;
    int val;
    int i =0 ;
    mem_file.open("data_mem_7_with.txt");
    while(getline(mem_file,line)){
        if(i==mem_addr){
            val  =stoi(line);
            mem_file.close();
            DM_DM[PC+1].push_back(val);
            return val;
        }else{
            i++;
        }
    }
    return 0; 
}

void memory_1::init(){
    ofstream reg_file;
    reg_file.open("data_mem_7_with.txt");
    for(int i = 0; i <1023; i++){
        reg_file<<0<<endl;
    }
    reg_file<<0;
    reg_file.close();
};

// -------------------------------------------- ******************** ----------------------------------- //

class memory_2{
    public : 
        void store(int reg_addr, int value);
        void write(int value, int offset, int PC);
};

// ============================================ FUNCTION DECLARATIONS ====================================== //

void memory_2::store(int reg_addr, int value){
    string line;
    fstream reg_file;
    reg_file.open("data_mem_7_with.txt");
    vector<string> temporary;
    while(getline(reg_file, line)){
        temporary.push_back(line);
    }
    reg_file.close();
    ofstream updated_mem;
    updated_mem.open("data_mem_7_with.txt");
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

void memory_2::write(int value, int offset,  int PC){
    string line;
    fstream mem_file;
    mem_file.open("data_mem_7_with.txt");
    vector<int> temp;
    while (getline(mem_file,line)){
        temp.push_back(stoi(line));
    }
    mem_file.close();
    temp.push_back(value);
    mem_file.open("data_mem_7_with.txt",fstream::out| fstream::trunc);
    for(int i = 0; i<temp.size();i++){
        mem_file << temp[i] <<endl;
    }
    mem_file.close();
}
