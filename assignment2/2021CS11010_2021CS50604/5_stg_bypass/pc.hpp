//pc.h

#ifndef _PC_H
#define _PC_H
using namespace std;

class pc{
	public:
	pc() {setPC(0);} //assign prg ctr to 0 
	void inc(){prg_ctr++;} // increase prg_ctr
	void setPC(int input){prg_ctr = input;} //set pc to some vales
	int getPC(){return prg_ctr;} // get pc value
	
    void findWhenToJump(){is_jump = prg_ctr + 1;} //Gives enough time for any current running operation to finish (any current op only needs to at most use both MEM and WB)
	void setWhenToJump(int IN){is_jump = IN;}
	void setJumpTo(int jump){jump_to = jump;}
	int whenToJump(){return is_jump;}
	int jumpTo(){return jump_to;}
	
    private:
	int prg_ctr;
	int jump_to; //Tells what line to jump to on jtype or branch
	int is_jump = -1; //Tells what PC value to jump / branch at
};
#endif