# COL216_assignment
All assignments of col216( programming languages) given in semester 2 of academic year 2022-2023


ASSIGNMENT 1 :
PART1 : Write a MIPS assembly program to print the input string after saving it to the heap.
PART2 :
Write a MIPS assembly program to perform an iterative binary search to check if an element exists in a sorted array.
PART 3: Write a MIPS assembly program to perform a recursive computation of x^n.


ASSIGNMENT 2:
The goal of the assignment was to simulate a 5 stage and 7-9 stage pipeline in C++. Following is the design decision made by us in our implementation.
1) We have implemented 23 instructions in our simulator.
2) We had made a header file for each state, and they have used
it in the main files. The header files of stage 5 without forwarding were reused in with forwarding with slight modifications. The functions in the header files were split accoringly to make a 7-9 pipeline. IF EX and WB were used in all the main files as it is.
3) We have made a test file for registers value and data memory for easy debugging.
4) A new instruction will begin from a new line
5) The register name in the instruction can be either the
standard MIPS register name or it can be $0, $1 to $ 31. Note
     that one cannot write in a $zero or $0 register.
6) A special instruction is being added called “end” which will
denote the end of the instruction set. Users need not write
     “end”, it will automatically be appended by our program.
7) Instruction decode state just decodes the values of the
     register and checks for stalls and forwarding.
8) All the branch decisions are made in the Execution stage.
9) If there is a jump instruction, then the instruction of the
destination can be fetched in the same cycle. Thus, it reduces the cycle count.

ASSIGNMENT 3:
1) We have used the inclusion property. L1 cache is contained in L2 cache.
2) If we get a write request ,that will be completed in the same time as a
read request will be processed.
3) L2 cache is not directly accessed from the main block. All the requests
first go through L1 cache and this cache redirects it to L2 if required.
We have tried to implement the cache very similar to what is available in hardware.
Following policy is used for the cache design
1) If L1 hits then, increase the counter if read_hit by one and change the age(how recently the value is used) to the new value.
2) If it is a L1 miss , then we raise a L1 write request. If the block is present in L2 then it is loaded in L1 and given the most recent value of age in L2 and L1. If the loading block replaces a block in L1 then we raise a L2 write request to write this replaced block in L2. and load the required word in L1.
3) IfablockisreplacedfromL2,thenifacopyofitispresentinL1,itis invalidated by turning its valid bit on.
4) IfwegetaL2miss,thenweloadthedatainL2andL1tosatisfythe inclusion property.
