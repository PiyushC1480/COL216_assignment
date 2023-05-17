.text
.globl main

main:
    li $v0 ,4
    la $a0,msg1
    syscall

    li $v0,5
    syscall
    move $t0,$v0

    mult $t0,$t0
    mflo $t1
    li $v0,4
    la $a0,msg2
    syscall
    li $v0,1
    move $a0,$t1
    syscall
    li $v0,10
    syscall

.data
msg1: .asciiz "enter n"
msg2: .asciiz "square is:"