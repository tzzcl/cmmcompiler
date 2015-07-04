.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
t0:.word 0
t1:.word 0
t2:.word 0
t3:.word 0
t4:.word 0
t5:.word 0
t6:.word 0
t7:.word 0
t8:.word 0
t9:.word 0
t10:.word 0
t11:.word 0
t12:.word 0
t13:.word 0
t14:.word 0
t15:.word 0
t16:.word 0
t17:.word 0
t18:.word 0
t19:.word 0
t20:.word 0
t21:.word 0
t22:.word 0
t23:.word 0
t24:.word 0
t25:.word 0
t26:.word 0
t27:.word 0
t28:.word 0
t29:.word 0
t30:.word 0
t31:.word 0
t32:.word 0
t33:.word 0
t34:.word 0
t35:.word 0
t36:.word 0
t37:.word 0
t38:.word 0
t39:.word 0
t40:.word 0
t41:.word 0
t42:.word 0
t43:.word 0
t44:.word 0
t45:.word 0
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

