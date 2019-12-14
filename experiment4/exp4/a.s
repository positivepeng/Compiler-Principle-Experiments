.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra
fact:
	lw $t0, 0($sp)
	addi $sp, $sp, 4
	li $t1 1
	beq $t0, $t1, label0
	j label1
label0:
	move $v0, $t0
	jr $ra
	j label2
label1:
	li $t2 1
	sub $t3, $t0, $t2
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	jal fact
	lw $t0, 0($sp)
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $t4, $v0
	mul $t5, $t0, $t4
	move $v0, $t5
	jr $ra
label2:
main:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $t6, $v0
	move $t7, $t6
	li $t8 1
	bgt $t7, $t8, label3
	j label4
label3:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	addi $sp, $sp, -4
	sw $t7, 0($sp)
	jal fact
	lw $t0, 0($sp)
	addi $sp, $sp, 4
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $t9, $v0
	move $t0, $t9
	j label5
label4:
	li $t0 1
	move $t0, $t0
label5:
	move $a0, $t0
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $t0 0
	move $v0, $t0
	jr $ra
