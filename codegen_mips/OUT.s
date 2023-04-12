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
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, 8($sp)
main_file_ret
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	addi $sp, $sp, 4
	jr $ra
.text
main2:
	addi $sp, $sp, -4
	sw $fp, 0($sp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $fp, 8($sp)
main2_file_ret
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	addi $sp, $sp, 4
	jr $ra
