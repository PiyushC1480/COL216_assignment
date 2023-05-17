# Print string stored in heap
.data
prompt: .asciiz "Enter a string: "

.text
.globl main
main:
  # Print the  prompt
  li $v0, 4
  la $a0, prompt
  syscall

  # Read the string from user
  li $v0, 9      # invoke sbrk system call
  li $a0, 100    # allocate 100 bytes of memory on the heap
  syscall        # invoke sbrk system call to allocate memory

  move $s0, $v0  # store the pointer to beginning of allocated memory in save register $s0

  li $v0, 8      # set $v0 to 8, indicating read string system call
  move $a0, $s0  # set $a0 to point to beginning of allocated memory
  li $a1, 100    # set $a1 to maximum number of characters to read, that is 100
  syscall 

  # Print string
  li $v0, 4      # set $v0 to 4 to indicate print string system call
  move $a0, $s0  # set $a0 to point to the beginning of the string in memory
  syscall        # invoke system call to print the string

  # Exit program
  li $v0, 10
  syscall