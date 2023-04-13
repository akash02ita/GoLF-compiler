# mips code
.text
main:
	# Save old fp and lr
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, $sp, 8
	move $t0, $zero   # constant write false
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_0
	move $t0, $zero   # constant write false
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_0:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_0: .byte  9, 99, 104, 101, 99, 107, 32, 97, 110, 100, 10, 0
.text
	la $t0, string_0 # loading address of 1st char of string `\tcheck and\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1   # constant write true
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_1
	move $t0, $zero   # constant write false
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_1:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_1: .byte  9, 99, 104, 101, 99, 107, 32, 97, 110, 100, 10, 0
.text
	la $t0, string_1 # loading address of 1st char of string `\tcheck and\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	move $t0, $zero   # constant write false
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_2
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_2:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_2: .byte  9, 99, 104, 101, 99, 107, 32, 97, 110, 100, 10, 0
.text
	la $t0, string_2 # loading address of 1st char of string `\tcheck and\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1   # constant write true
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_3
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_3:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_3: .byte  9, 99, 104, 101, 99, 107, 32, 97, 110, 100, 10, 0
.text
	la $t0, string_3 # loading address of 1st char of string `\tcheck and\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
.data
.align 2
string_4: .byte  10, 0
.text
	la $t0, string_4 # loading address of 1st char of string `\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	move $t0, $zero   # constant write false
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	bne $t0, $zero, skipor_true_4
	move $t0, $zero   # constant write false
	lw $t1, 0($sp)
	or $t0, $t1, $t0
skipor_true_4:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_5: .byte  9, 99, 104, 101, 99, 107, 32, 111, 114, 10, 0
.text
	la $t0, string_5 # loading address of 1st char of string `\tcheck or\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1   # constant write true
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	bne $t0, $zero, skipor_true_5
	move $t0, $zero   # constant write false
	lw $t1, 0($sp)
	or $t0, $t1, $t0
skipor_true_5:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_6: .byte  9, 99, 104, 101, 99, 107, 32, 111, 114, 10, 0
.text
	la $t0, string_6 # loading address of 1st char of string `\tcheck or\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	move $t0, $zero   # constant write false
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	bne $t0, $zero, skipor_true_6
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	or $t0, $t1, $t0
skipor_true_6:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_7: .byte  9, 99, 104, 101, 99, 107, 32, 111, 114, 10, 0
.text
	la $t0, string_7 # loading address of 1st char of string `\tcheck or\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1   # constant write true
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	bne $t0, $zero, skipor_true_7
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	or $t0, $t1, $t0
skipor_true_7:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_8: .byte  9, 99, 104, 101, 99, 107, 32, 111, 114, 10, 0
.text
	la $t0, string_8 # loading address of 1st char of string `\tcheck or\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
.data
.align 2
string_9: .byte  10, 0
.text
	la $t0, string_9 # loading address of 1st char of string `\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1 # base case int literal
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	li $t0, 2 # base case int literal
	addi $sp, $sp, -4
	sw $t0, 0($sp) # append lhs result
	# pop 2 words apply op
	lw $t0, 0($sp)
	addi $sp, $sp, 4
	lw $t1, 0($sp)
	addi $sp, $sp, 4
	add $t0, $t0, $t1
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	li $t0, 3 # base case int literal
	addi $sp, $sp, -4
	sw $t0, 0($sp) # append rhs result
	jal inteqeq
	move $t0, $v0  #  int relop result
	# pop 2 words rel op
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_8
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_8:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_10: .byte  9, 108, 105, 110, 101, 32, 51, 52, 10, 0
.text
	la $t0, string_10 # loading address of 1st char of string `\tline 34\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
.data
.align 2
string_11: .byte  104, 101, 108, 108, 111, 0
.text
	la $t0, string_11 # loading address of 1st char of string `hello`
	addi $sp, $sp, -4
	sw $t0, 0($sp)
.data
.align 2
string_12: .byte  104, 101, 108, 108, 111, 0
.text
	la $t0, string_12 # loading address of 1st char of string `hello`
	addi $sp, $sp, -4
	sw $t0, 0($sp) # append rhs result
	jal streqeq
	move $t0, $v0   # string relop result
	# pop 2 words rel op
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_9
	li $t0, 1   # constant write true
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_9:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_13: .byte  9, 108, 105, 110, 101, 32, 51, 53, 10, 0
.text
	la $t0, string_13 # loading address of 1st char of string `\tline 35\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	li $t0, 1   # constant write true
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	bne $t0, $zero, skipor_true_10
	jal main
	lw $t1, 0($sp)
	or $t0, $t1, $t0
skipor_true_10:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_14: .byte  9, 108, 105, 110, 101, 32, 51, 54, 10, 0
.text
	la $t0, string_14 # loading address of 1st char of string `\tline 36\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
	move $t0, $zero   # constant write false
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	beq $t0, $zero, skipand_false_11
	jal main
	lw $t1, 0($sp)
	and $t0, $t1, $t0
skipand_false_11:
	add $sp, $sp, 4
	# Adding arg #1 in functioncall printb()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal printb
	# Removing arg #2 in functioncall printb()
	addi $sp, $sp, 4
.data
.align 2
string_15: .byte  9, 108, 105, 110, 101, 32, 51, 54, 10, 0
.text
	la $t0, string_15 # loading address of 1st char of string `\tline 36\n`
	# Adding arg #1 in functioncall prints()
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal prints
	# Removing arg #2 in functioncall prints()
	addi $sp, $sp, 4
main_ret:
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
