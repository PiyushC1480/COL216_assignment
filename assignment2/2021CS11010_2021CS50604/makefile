compile :
	g++  ./new_pipeline/pc_5_without.hpp ./new_pipeline/if_5_without.hpp  ./new_pipeline/id_5_without.hpp  ./new_pipeline/alu_5_without.hpp ./new_pipeline/ex_5_without.hpp ./new_pipeline/mem_5_without.hpp ./new_pipeline/wb_5_without.hpp 
	g++ -o ./new_pipeline/test1 ./5stage.cpp 
	
	g++  ./5_stg_bypass/if.hpp  ./5_stg_bypass/id.hpp  ./5_stg_bypass/alu.hpp ./5_stg_bypass/ex.hpp ./5_stg_bypass/mem.hpp ./5_stg_bypass/wb.hpp 
	g++ -o ./5_stg_bypass/test2 ./5stage_bypass.cpp

	g++ ./79_stage_no_bypass/pc_7_without.hpp  ./79_stage_no_bypass/if_7_without.hpp  ./79_stage_no_bypass/id_7_without.hpp  ./79_stage_no_bypass/alu_7_without.hpp ./79_stage_no_bypass/ex_7_without.hpp ./79_stage_no_bypass/mem_7_without.hpp ./79_stage_no_bypass/read_7_without.hpp  ./79_stage_no_bypass/wb_7_without.hpp
	g++ -o ./79_stage_no_bypass/test3 ./79stage.cpp
	
	g++  ./79_stage_with_bypass/pc_7_with.hpp ./79_stage_with_bypass/if_7_with.hpp  ./79_stage_with_bypass/id_7_with.hpp  ./79_stage_with_bypass/alu_7_with.hpp ./79_stage_with_bypass/ex_7_with.hpp ./79_stage_with_bypass/mem_7_with.hpp ./79_stage_with_bypass/wb_7_with.hpp
	g++ -o ./79_stage_with_bypass/test4 ./79stage_bypass.cpp 

run_5stage :
	./new_pipeline/test1 input.asm
run_5stage_bypass:
	./5_stg_bypass/test2 input.asm
run_79stage:
	./79_stage_no_bypass/test3 input.asm
run_79stage_bypass:
	./79_stage_with_bypass/test4 input.asm