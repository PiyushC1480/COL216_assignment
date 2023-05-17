# faster square implementation
# x = x.(x^2)^((n-1)/2) if n is odd
#else x = (x^2)^(n/2)
#implementation is O(log n) because only half the time the recursion is called.
# below is the code which is translated to MIPS langiage.
# there is a reference to line numbers also from the below C code to the lines in MIPS code.

#1) int power_fast(int x, unsigned int y){
#2)     int temp;
#3)     if (y==0){
#4)         return 1;
#5)     }
#6)     else{
#7)         temp = power_fast(x,y/2);
#8)         if(y%2==0){
#9)             return temp*temp;
#10)         }else{
#11)             return x*temp*temp;
#12)         }
#13)     }
#14)  }

#below are the two prompts which asks user to enter the base and power and  prenits a message for answer
.data
prompt1: .asciiz "Enter the base x:"
prompt2: .asciiz "Enter the power n:"
msg: .asciiz "answer of x^n is :"
.text
.globl main
main:
    #below 3 lines prints the prompt1 
    li $v0,4
    la $a0,prompt1
    syscall
    #below 3 lines take the user input and moves it to a temporary register $t0
    li $v0,5
    syscall
    move $t0,$v0
    #below 3 lines prints the prompt1
    li $v0,4
    la $a0,prompt2
    syscall
    #below 3 lines take the user input and moves it to arguments register 
    li $v0,5
    syscall
    move $a1,$v0
    #this moves the base stored to $t0 to argument register
    move $a0,$t0
    #calls the power_expn function
    jal power_expn
    #move the answer stored in return register to temporary register
    move $t0,$v0
    #prints the message
    li $v0,4
    la $a0,msg
    syscall
    #prints the answer
    li $v0,1
    move $a0,$t0
    syscall
    #jump to exit 
    j exit


power_expn:
    #store the return address to stack
    addi $sp,$sp,-4
    sw $ra,0($sp)
    #corresponds to line 3 in C code . Checks whether the power is 0 or not, if yes then goes to return_1
    beq $a1,$zero,return_1
    #if power is not 0, then store it in stack
    addi $sp,$sp,-4
    sw $a1,0($sp)
    # divide the power by 2(see in line7 of code y/2 is used) and move it to the argument register
    srl $t0,$a1,1
    move $a1,$t0
    #call the power expn again(line 7 of C code)
    jal power_expn

    #load the power stored in the stack
    lw $a1,0($sp)
    #move stack pointer 
    addi $sp,$sp,4
    #move the value in the return register to $t1
    move $t1,$v0 
    # 
    andi $t2,$a1,1
    #if y is even then call return square
    beq $t2,$zero,return_sq  
    #otherwise multiply x with ($t1)^2 (line 11 of code) and store the answer in $v0
    mul $v0,$a0,$t1
    mul $v0,$v0,$t1
    #jump to returning value
    j return_val

return_1:
#called if power of the base is 0 
    # it loads 1 into the return redister(line 4 of code)
    li $v0,1
    #jump to the return_val 
    j return_val

return_sq:
    #similar ot line 9 of the C code
    # sqaures $t1 and puts it in the return register
    mul $v0,$t1,$t1
    #jump to the return_val 
    j return_val
return_val:
    #return to the return address by loading it and calling jr
    lw $ra,0($sp)
    addi $sp,$sp,4
    jr $ra

exit:
    #exit the code.
    li $v0,10
    syscall









