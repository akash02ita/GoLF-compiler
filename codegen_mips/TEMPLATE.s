
.data
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
    li $v0 4
    syscall
    jr $ra

printc: # input: $a0 = address of char to print
    li $v0 1
    syscall
    jr $ra

.data
.align 2
$true: .byte 116, 114, 117, 101, 0
$false: .byte 102, 97, 108, 115, 101, 0
$invalidbool: .byte 105, 110, 118, 97, 108, 105, 100, 32, 98, 111, 111, 108, 0
.text
.align2
printb: # input $0 = 0 or 1 (an integer)
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    li $t0, 0
    beq $a0, $t0, printb_true
    li $t0, 1
    beq $a0, $t0, printb_false


    la $a0, $invalidbool
    jal prints
    j printb_ret

printb_true:
    la $a0, $true
    jal prints
    j printb_ret
printb_false:
    la $a0, $false
    jal prints
printb_ret:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra


printi:
    li $v0 1
    syscall
    jr $ra


len: # input address of first char
    move $v0, $zero # counter = 0
    j lentest
lenloop:
    addi $a0, $a0, 1 // next char
    addi $v0, $v0, 1 // length++
lentest:
    lb $t0, 0($a0)
    bne $t0, $zero, lenloop

    jr $ra


halt:
    li $v0, 10
    syscall

