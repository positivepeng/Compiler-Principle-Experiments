FUNCTION fact :
PARAM t3
t7 := #1
IF t3 == t7 GOTO label0
GOTO label1
LABEL label0 : 
RETURN t3
GOTO label2
LABEL label1 : 
t8 := #1
t9 := t3 - t8
t10 := #1
t11 := t3 - t10
ARG t11
t12 := CALL fact
t13 := t3 * t12
RETURN t13
LABEL label2 : 
FUNCTION main :
READ t14
t5 := t14
t15 := #1
IF t5 > t15 GOTO label3
GOTO label4
LABEL label3 : 
ARG t5
t16 := CALL fact
t6 := t16
GOTO label5
LABEL label4 : 
t17 := #1
t6 := t17
LABEL label5 : 
WRITE t6
t18 := #0
RETURN t18
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
	li $t0 1
	beq $t1, $t0, label0
	j label1
label0:
	move $v0, $t2
	jr $ra
	j label2
label1:
	li $t2 1
	sub $t3, $t1, $t2
	li $t4 1
	sub $t5, $t1, $t4
ARG : t11:
	jal fact
	move $t6 $v0
	mul $t7, $t1, $t6
	move $v0, $t8
	jr $ra
label2:
main:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	move $t8, $v0
	move $t9, $t8
	li $t0 1
	bgt $t9, $t0, label3
	j label4
label3:
ARG : t5:
	jal fact
	move $t0 $v0
	move $t0, $t0
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
