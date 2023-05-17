.data
# propmpts for the user  -----------------------------------------------------------------------------------
prompt: .asciiz "Enter the size of the array: "             
array_prompt: .asciiz "Enter the elements of the array : "
search_prompt: .asciiz "Enter the element to search for: "
found_p: .asciiz "Yes at index = "
not_found_p: .asciiz "element not found"
# -----------------------------------------------------------------------------------------------------------

.text
.globl main

main:
  # Print prompt to get array size
  li $v0, 4
  la $a0, prompt
  syscall

  # Read array size
  li $v0, 5
  syscall
  move $s0, $v0                            # $s0 stores length of the array(n), i.e. $s0 = n

  # Print prompt to get array elements
  li $v0, 4
  la $a0, array_prompt
  syscall

  # allocate space for array in heap
  sll $t0, $s0, 2                          # $t0 = $s0 * 4 
  move $a0, $t0                            # $a0 = $t0
  li $v0, 9          
  syscall
  move $a1, $v0                            # $a1 = $v0, $a1 is the base address of the array

  # Read array elements
  li $t1, 0                                # $t1 = 0, $t1 is the counter

read_loop:
    beq $t1, $s0 , done_reading            # if $t1 == $s0 (counter == n) then go to "done_reading"
    li $v0, 5                        
    syscall
    sll  $t2, $t1, 2                       # $t2 = $t1 * 4
    add  $t2, $t2, $a1                     # $t2 = $t2 + $a1, $t2 stores the address where the current arr element will be stored
    sw  $v0 , 0($t2)                       # $v0 = arr element input by the user
    addi $t1, $t1, 1                       # $t1 = $t1 + 1
    j read_loop                            # jump to "read_loop"

done_reading:  li $v0, 4                   # prompt for search element, x
               la $a0, search_prompt       
               syscall
               li $v0, 5                   # input x from the user stored in $v0
               syscall
               move $a2, $v0               # $a2 = $v0
               move $a0, $s0               # $a0 = $s0 (n)
               jal BS                      # jump to BS (binary search function)
               move $s0, $v0               # $s0 = $v0
               addi $t0, $zero, -1         # $t0 = -1
               beq  $s0, $t0, not_found    # if ($s0 = -1) then not_found 
               li, $v0, 4                  # prompt found_p
               la $a0, found_p
               syscall
               li $v0, 1                   # output the result
               move $a0, $s0               
               syscall
               j FINAL_EXIT                # jump to "FINAL_EXIT"


# --------------------------- BINARY SEARCH FUNCTION ------------------------------------------------------
# arguments to function :  $a0 = n, $a1 = base address of arr, $a2 = x
BS: addi  $sp, $sp, -16                    # allocate space on stack 
    sw    $s3, 12($sp)                     # store saved registers in stack 
    sw    $s2, 8($sp)
    sw    $s1, 4($sp)
    sw    $s0, 0($sp)                      # $s0, $s1, $s2, $s3 stored in stack
    move  $s0, $zero                       # $s0 = 0 
    addi  $s1, $a0, -1                     # $s1 = $a0 - 1 
    bne   $s0, $s1, LOOP                   # if ($s0 = $s1) then EXIT else LOOP
    beq   $s0, $s1, EXIT

# here $s0 is a and $s1 is b, $s2 is mid, #s3 is arr[mid]

LOOP: add   $t1, $s0, $s1                  # $t1 = $s0 + $s1 = (a+b)
      srl   $s2, $t1, 1                    # $s2 = ($t1)// 2 = mid
      sll   $t1, $s2, 2                    # $t1 = mid*4
      add   $t1, $t1, $a1                  # $t1 = $t1 + $a1, $t1 is the address of arr[mid]
      lw    $s3, 0($t1)                    # $s3 = arr[mid]
      beq   $s3, $a2, Return1              # if x = arr[mid] then Return1
      slt   $t0, $s3, $a2                  # if (x > arr[mid]) then $t0 = 1 else $t0 = 0
      bne   $t0, $zero, A                  # if ($t0 != 0) then A
      addi  $s1, $s2, -1                   # b = mid - 1
      #  #  #  #  #  #  #                  # Loop conditions
      slt   $t2, $s0, $a0                  # if (a < n) then $t2 = 1 else $t2 = 0 
      beq   $t2, $zero, EXIT               # if ($t2 == 0) then "EXIT"
      slt   $t2, $s1, $zero                # if (b < 0) then $t2 = 1 else $t2 = 0
      bne   $t2, $zero, EXIT               # if ($t2 != 0) then "EXIT"
      beq   $s0, $s1, EXIT                 # if (a == b) then "EXIT"
      bne   $s0, $s1, LOOP                 # else "LOOP"
      #  #  #  #  #  #  #  #
     

Return1:  add $v0, $zero, $s2              # $v0 = mid 
          lw    $s0, 0($sp)                # restore saved registers from stack 
          lw    $s1, 4($sp)
          lw    $s2, 8($sp)
          lw    $s3, 12($sp)
          addi  $sp, $sp, 16               # clear stack
          jr $ra                           # return

Return2:  add $v0, $zero, $s0              # $v0 = $s0 (return a)
          lw    $s0, 0($sp)                # restore saved registers from stack 
          lw    $s1, 4($sp)                
          lw    $s2, 8($sp)
          lw    $s3, 12($sp)      
          addi  $sp, $sp, 16               # clear stack
          jr $ra                           # return 

A: addi  $s0, $s2, 1
      #  #  #  #  #  #  #                  # Loop conditions
      slt   $t2, $s0, $a0                  # if (a < n) then $t2 = 1 else $t2 = 0 
      beq   $t2, $zero, EXIT               # if ($t2 == 0) then "EXIT"
      slt   $t2, $s1, $zero                # if (b < 0) then $t2 = 1 else $t2 = 0
      bne   $t2, $zero, EXIT               # if ($t2 != 0) then "EXIT"
      beq   $s0, $s1, EXIT                 # if (a == b) then "EXIT"
      bne   $s0, $s1, LOOP                 # else "LOOP"
      #  #  #  #  #  #  #  #
   
    
EXIT: sll $t0, $s0, 2                     # $t0 = a*4
      add $t0, $t0, $a1                   # $t0 = $t0 + $a1, $t0 = address of arr[a]
      lw  $s3, 0($t0)                     # $s3 = arr[a]
      beq  $a2, $s3, Return2              # if (x == arr[a]) then "Return2"
      addi $v0, $zero, -1                 # $v0 = -1
      lw    $s0, 0($sp)                   # restore saved registers from stack 
      lw    $s1, 4($sp)
      lw    $s2, 8($sp)
      lw    $s3, 12($sp)
      addi  $sp, $sp, 16                 # clear stack
      jr $ra                             # return
# --------------------------------------------------------------------------------------------------------------

not_found: li, $v0, 4                    # prompt not_found_p
           la $a0, not_found_p
           syscall
           j FINAL_EXIT                  # jump to "FINAL_EXIT"

FINAL_EXIT: li $v0, 10                   # end 
            syscall
