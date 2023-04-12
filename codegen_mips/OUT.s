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
	addi $fp, 8($sp)
	# Alloc int_file_main
	sw $zero, 0($sp)
	# Alloc test_file_main
	la $t0, $string_zero
	sw $t0, 0($sp)
main_file_ret
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
	addi $fp, 8($sp)
	# Alloc int_file_main2
	sw $zero, 0($sp)
	# Alloc test_file_main2
	la $t0, $string_zero
	sw $t0, 0($sp)
main2_file_ret
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
