# mips code
.data
.align 2
x_file:	.word $string_empty
.data
.align 2
glob_file:	.word $string_empty
.data
.align 2
integer_file:	.word 0
.text
main:
	# Save old fp and lr
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, $sp, 8
	# Alloc integer2_file_main
	addi $sp, $sp, -4  # add memory for lv
	sw $zero, 0($sp)
	# Alloc x_file_main_block
	addi $sp, $sp, -4  # add memory for lv
	la $t0, $string_empty
	sw $t0, 0($sp)
	# Alloc y_file_main_block
	addi $sp, $sp, -4  # add memory for lv
	la $t0, $string_empty
	sw $t0, 0($sp)
	# Alloc x_file_main_block_block
	addi $sp, $sp, -4  # add memory for lv
	la $t0, $string_empty
	sw $t0, 0($sp)
	li $t0, 5
	sw $t0, -12($fp) # assignment for integer2 int
.data
.align 2
string_0: .byte  72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 10, 0
.text
	la $t0, string_0 # loading address of 1st char of string `Hello, world!\n`
	la $t1, x_file
	sw $t0, 0($t1) # assignment glob var x
	la $t0, integer_file # load content of glob var integer int
	lw $t0, 0($t0) # load address of 1st char
	# Adding arg #1 in functioncall printi()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printi
	# Removing arg #2 in functioncall printi()
	addi $sp, $sp, 4
.data
.align 2
string_1: .byte  10, 0
.text
	la $t0, string_1 # loading address of 1st char of string `\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	lw $t0, -12($fp) # load content of integer2 int
	# Adding arg #1 in functioncall printi()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printi
	# Removing arg #2 in functioncall printi()
	addi $sp, $sp, 4
.data
.align 2
string_2: .byte  10, 0
.text
	la $t0, string_2 # loading address of 1st char of string `\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
.data
.align 2
string_3: .byte  104, 111, 108, 97, 32, 118, 97, 109, 111, 115, 10, 0
.text
	la $t0, string_3 # loading address of 1st char of string `hola vamos\n`
	la $t1, glob_file
	sw $t0, 0($t1) # assignment glob var glob
.data
.align 2
string_4: .byte  10, 9, 100, 111, 32, 110, 111, 116, 32, 112, 114, 105, 110, 116, 32, 111, 117, 116, 115, 105, 100, 101, 32, 115, 99, 111, 112, 101, 32, 116, 104, 105, 115, 10, 0
.text
	la $t0, string_4 # loading address of 1st char of string `\n\tdo not print outside scope this\n`
	sw $t0, -16($fp) # assignment for x string
.data
.align 2
string_5: .byte  104, 101, 108, 108, 111, 32, 116, 104, 105, 115, 32, 105, 115, 32, 97, 32, 116, 101, 115, 116, 10, 0
.text
	la $t0, string_5 # loading address of 1st char of string `hello this is a test\n`
	sw $t0, -20($fp) # assignment for y string
	lw $t0, -16($fp) # load content of x string
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 234324
	la $t1, integer_file
	sw $t0, 0($t1) # assignment glob var integer
.data
.align 2
string_6: .byte  119, 104, 97, 116, 101, 118, 101, 114, 0
.text
	la $t0, string_6 # loading address of 1st char of string `whatever`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
.data
.align 2
string_7: .byte  97, 110, 111, 116, 104, 101, 114, 32, 115, 99, 111, 112, 101, 45, 45, 45, 45, 10, 0
.text
	la $t0, string_7 # loading address of 1st char of string `another scope----\n`
	sw $t0, -24($fp) # assignment for x string
	lw $t0, -24($fp) # load content of x string
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	lw $t0, -20($fp) # load content of y string
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	lw $t0, -20($fp) # load content of y string
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	lw $t0, -16($fp) # load content of x string
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	la $t0, glob_file # load content of glob var glob string
	lw $t0, 0($t0) # load address of 1st char
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	la $t0, integer_file # load content of glob var integer int
	lw $t0, 0($t0) # load address of 1st char
	# Adding arg #1 in functioncall printi()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printi
	# Removing arg #2 in functioncall printi()
	addi $sp, $sp, 4
.data
.align 2
string_8: .byte  45, 45, 45, 45, 10, 0
.text
	la $t0, string_8 # loading address of 1st char of string `----\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	la $t0, x_file # load content of glob var x string
	lw $t0, 0($t0) # load address of 1st char
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	la $t0, glob_file # load content of glob var glob string
	lw $t0, 0($t0) # load address of 1st char
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
main_ret:
	# Dealloc integer2_file_main
	add $sp, $sp, 4
	# Dealloc x_file_main_block
	add $sp, $sp, 4
	# Dealloc y_file_main_block
	add $sp, $sp, 4
	# Dealloc x_file_main_block_block
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

