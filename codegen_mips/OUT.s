# mips code
.data
.align 2
anotherglobvar_file:	.word $string_empty
.data
.align 2
anotherglobvar2_file:	.word 0
.data
.align 2
globalvar_file:	.word 0
.text
main:
	# Save old fp and lr
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, $sp, 8
	# Alloc int_file_main
	addi $sp, $sp, -4  # add memory for lv
	sw $zero, 0($sp)
	# Alloc test_file_main
	addi $sp, $sp, -4  # add memory for lv
	la $t0, $string_empty
	sw $t0, 0($sp)
ifbody_0:
ifbody_1:
ifelse_1:
ifbody_2:
ifelse_2:
ifelse_0:
forpretest_0:
forloop_0:
forpretest_1:
forloop_1:
	j forexitloop_1
	j forpretest_1
forexitloop_1:
	move $v0, $t0
	j main_file_ret
	j forpretest_0
forexitloop_0:
main_file_ret:
	# Dealloc int_file_main
	add $sp, $sp, 4
	# Dealloc test_file_main
	add $sp, $sp, 4
	# Restore old fp and lr
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	addi $sp, $sp, 4
	# return to caller
	jr $ra
.text
main2:
	# Save old fp and lr
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, $sp, 8
	# Alloc int_file_main2
	addi $sp, $sp, -4  # add memory for lv
	sw $zero, 0($sp)
	# Alloc test_file_main2
	addi $sp, $sp, -4  # add memory for lv
	la $t0, $string_empty
	sw $t0, 0($sp)
main2_file_ret:
	# Dealloc int_file_main2
	add $sp, $sp, 4
	# Dealloc test_file_main2
	add $sp, $sp, 4
	# Restore old fp and lr
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	addi $sp, $sp, 4
	# return to caller
	jr $ra

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

