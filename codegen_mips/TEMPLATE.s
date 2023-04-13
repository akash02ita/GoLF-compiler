
.data
input:  .space 2
        .align 2

$string_empty: .byte 0 # by default string will be initialized to empty strings

.align 2
.text

# RTS FUNCTIONS
getchar:
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    li $v0, 8                       # System call for read_string
    la $a0, input                   # Pass in buffer (size = n+1)
    li $a1, 2                       # Size = n+1 as read_string adds null
    syscall

    lb $v0, input
    bne $v0, $zero, getchar_ret

    li $v0, -1                      # If 0, map to -1 and return
getchar_ret:
    lw $ra 0($sp)
    addi $sp $sp, 4
    jr $ra


prints: # input: $a0 = address of first char of string
    lw $a0, 0($sp) # in my case, i am passing all args in stack
    li $v0 4
    syscall
    jr $ra

printc: # input: $a0 = address of char to print
    lw $a0, 0($sp) # in my case, i am passing all args in stack
    li $v0 1
    syscall
    jr $ra

.data
.align 2
$true: .byte 116, 114, 117, 101, 0
$false: .byte 102, 97, 108, 115, 101, 0
$invalidbool: .byte 105, 110, 118, 97, 108, 105, 100, 32, 98, 111, 111, 108, 0
.text
.align 2
printb: # input $0 = 0 or 1 (an integer)
    lw $a0, 0($sp) # in my case, i am passing all args in stack
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    li $t0, 0
    beq $a0, $t0, printb_false
    li $t0, 1
    beq $a0, $t0, printb_true


    la $a0, $invalidbool
    addi $sp, $sp, -4
    sw $a0, 0($sp)
    jal prints
    addi $sp, $sp, 4
    j printb_ret

printb_true:
    la $a0, $true
    addi $sp, $sp, -4
    sw $a0, 0($sp)
    jal prints
    addi $sp, $sp, 4
    j printb_ret
printb_false:
    la $a0, $false
    addi $sp, $sp, -4
    sw $a0, 0($sp)
    jal prints
    addi $sp, $sp, 4
printb_ret:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra


printi:
    lw $a0, 0($sp) # in my case, i am passing all args in stack
    li $v0 1
    syscall
    jr $ra


len: # input address of first char
    lw $a0, 0($sp) # in my case, i am passing all args in stack
    move $v0, $zero # counter = 0
    j lentest
lenloop:
    addi $a0, $a0, 1 # next char
    addi $v0, $v0, 1 # length++
lentest:
    lb $t0, 0($a0)
    bne $t0, $zero, lenloop

    jr $ra


halt:
    li $v0, 10
    syscall

# ------------------------ comparison subroutines
rettrue:
    li $v0, 1
    jr $ra
retfalse:
    move $v0, $zero
    jr $ra


        # ----------------
intlt:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    blt $t0, $t1, rettrue
    j retfalse
intgt:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    bgt $t0, $t1, rettrue
    j retfalse
intlte:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    ble $t0, $t1, rettrue
    j retfalse
intgte:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    bge $t0, $t1, rettrue
    j retfalse
inteqeq:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    beq $t0, $t1, rettrue
    j retfalse
intneq:
    lw $t0, 4($sp)
    lw $t1, 0($sp)
    bne $t0, $t1, rettrue
    j retfalse
        # ----------------

strcmp: # inputs: $a0, $a1
    lb $t0, ($a0)      
    lb $t1, ($a1)      

strcmploop:
    bne $t0, $t1, strcmpcheck
    beqz $t0, strcmpcheck

    addi $a0, $a0, 1   # next char address
    addi $a1, $a1, 1   # next char address
    lb $t0, ($a0)      # load char
    lb $t1, ($a1)      # load char
    j strcmploop

strcmpcheck:
    sub $v0, $t0, $t1  # difference will tell if string are equal, 1st string is less than (neg) or greater than (pos) than 2nd string
    jr $ra             # return to the caller

# -----------------------------------------


strlt:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal intlt
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
strgt:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal intgt
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
strlte:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal intlte
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
strgte:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal intgte
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
streqeq:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal inteqeq
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
strneq:
    lw $a0, 4($sp)
    lw $a1, 0($sp)
    # save stack frame
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # code
    jal strcmp
    addi $sp, $sp, -4
    sw $v0, 0($sp)
    addi $sp, $sp, -4
    sw $zero, 0($sp)
    jal intneq
    add $sp, $sp, 4
    add $sp, $sp, 4
    # restore stack frame
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
