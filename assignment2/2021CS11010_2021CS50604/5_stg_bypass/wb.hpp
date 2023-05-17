#include <iostream>
#include <fstream> 
#include <vector>
using namespace std; 

class wb{
    public: 
        void write_back(int value, int reg_addr);
};

void wb::write_back(int value, int reg_addr){
    string line;
    fstream reg_file;
    reg_file.open("reg.txt");
    vector<int> temporary;
    while(getline(reg_file, line)){
        temporary.push_back(stoi(line));
    }
    reg_file.close();
    int new_val = value;
    ofstream updated_reg;
    updated_reg.open("reg.txt");
    for(int line_no=0; line_no<temporary.size(); line_no++){
        if (line_no == reg_addr){
            updated_reg<<new_val<<endl;
        }
        else{
            updated_reg<<temporary[line_no]<<endl;
        }
    }
    updated_reg.close();
}