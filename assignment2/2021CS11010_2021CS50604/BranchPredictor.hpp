#ifndef __BRANCH_PREDICTOR_HPP__
#define __BRANCH_PREDICTOR_HPP__

#include <vector>
#include <bitset>
#include <cassert>

struct BranchPredictor {
    virtual bool predict(uint32_t pc) = 0;
    virtual void update(uint32_t pc, bool taken) = 0;
};

struct SaturatingBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2> > table;
    SaturatingBranchPredictor(int value) : table(1 << 14, value) {}

    bool predict(uint32_t pc) {
		uint32_t index = pc & 0x3FFF; // last 14 bit extracted from PC
        std::bitset<2>& counter = table[index]; // Get the saturating counter value from the table
        bool prediction = counter.test(1); // Predict the outcome of the branch based on the saturating counter value
        return prediction; // returnning the predicted outcome;
	}

	void update(uint32_t pc, bool taken) {
		uint32_t index = pc & 0x3FFF;  // last 14 bit extracted from PC
        std::bitset<2>& counter = table[index]; // Get the saturating counter value from the table
        // Update the saturating counter based on the actual outcome of the branch
        if (taken) {
            if (counter == 0b00) {
                counter = 0b01;
            } else if (counter == 0b01 ) {
                counter = 0b10;
            }else if(counter == 0b10 ){
				counter = 0b11;
			}else{
				counter = 0b11;
			}
        } else {
            if (counter == 0b11) {
                counter = 0b10;
            } else if (counter == 0b10) {
                counter = 0b01;
            }else if(counter == 0b01 ){
				counter = 0b00;
			}else{
				counter = 0b00;
			}
        }
		table[index] = counter;
	}
};

struct BHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    BHRBranchPredictor(int value) : bhrTable(1 << 2, value), bhr(value) {}

    bool predict(uint32_t pc) {
		// Extract the index from the PC
        uint32_t index = pc & 0b11;
        
        // Get the prediction from the table at the indexed location
        bool prediction = bhrTable[bhr.to_ulong()].test(1);
        
        return prediction;
	}

	void update(uint32_t pc, bool taken) {
		uint32_t index = pc & 0b11;
        
        // Update the table at the indexed location
        if (taken) {
            if (bhrTable[bhr.to_ulong()] == 0b00){
                bhrTable[bhr.to_ulong()]  = 0b01;
            }else if (bhrTable[bhr.to_ulong()] == 0b01){
                bhrTable[bhr.to_ulong()]  = 0b10;
            } else if (bhrTable[bhr.to_ulong()] == 0b10){
                bhrTable[bhr.to_ulong()]  = 0b11;
            }else {
                bhrTable[bhr.to_ulong()]  = 0b11;
            }
        } else {
            if (bhrTable[bhr.to_ulong()] == 0b00){
                bhrTable[bhr.to_ulong()]  = 0b00;
            }else if (bhrTable[bhr.to_ulong()] == 0b01){
                bhrTable[bhr.to_ulong()]  = 0b00;
            } else if (bhrTable[bhr.to_ulong()] == 0b10){
                bhrTable[bhr.to_ulong()]  = 0b01;
            }else {
                bhrTable[bhr.to_ulong()]  = 0b10;
            }
        }
        // Update the BHR
        bhr <<= 1;
        bhr.set(0, taken);
    }
};

struct SaturatingBHRBranchPredictor : public BranchPredictor {
    std::vector<std::bitset<2>> bhrTable;
    std::bitset<2> bhr;
    std::vector<std::bitset<2>> table;
    std::vector<std::bitset<2>> combination;
    SaturatingBHRBranchPredictor(int value, int size) : bhrTable(1 << 2, value), bhr(value), table(1 << 14, value), combination(size, value) {
        assert(size <= (1 << 16));
    }
    bool predict(uint32_t pc) {
        std::bitset<2> tablePrediction = table[pc & ((1 << 14) - 1)];
        std::bitset<2> bhrTablePrediction = bhrTable[bhr.to_ulong() & ((1 << 2) - 1)];
        int combinationIndex = (tablePrediction.to_ulong() << 1) | bhrTablePrediction.to_ulong();
        std::bitset<2> combinationPrediction = combination[combinationIndex];
        return combinationPrediction.test(1);
    }

    void update(uint32_t pc, bool taken) {
        std::bitset<2> tablePrediction = table[pc & ((1 << 14) - 1)];
        std::bitset<2> bhrTablePrediction = bhrTable[bhr.to_ulong() & ((1 << 2) - 1)];
        int combinationIndex = (tablePrediction.to_ulong() << 1) | bhrTablePrediction.to_ulong();
        std::bitset<2>& combinationEntry = combination[combinationIndex];

        // Update combination entry based on the outcome of the branch
        if (taken) {
            if (combinationEntry.test(1)) {
                combinationEntry.set(0, true);
                combinationEntry.set(1, true);
            } else if (combinationEntry.test(0)) {
                combinationEntry.set(0, false);
                combinationEntry.set(1, true);
            } else {
                combinationEntry.set(0, true);
                combinationEntry.set(1, false);
            }
        } else {
            if (combinationEntry.test(1)) {
                combinationEntry.set(0, true);
                combinationEntry.set(1, false);
            } else if (combinationEntry.test(0)) {
                combinationEntry.set(0, false);
                combinationEntry.set(1, false);
            } else {
                combinationEntry.set(0, true);
                combinationEntry.set(1, true);
            }
        }

        combination[combinationIndex] = combinationEntry;
        // Update table and BHR based on the outcome of the branch
        table[pc & ((1 << 14) - 1)] = tablePrediction >> 1;
        table[pc & ((1 << 14) - 1)].set(1, taken);
        bhr<<1;
        bhr.set(0,taken);
    }
};
    
    
#endif