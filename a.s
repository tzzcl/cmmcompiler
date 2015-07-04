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
t46:.word 0
t47:.word 0
t48:.word 0
t49:.word 0
t50:.word 0
t51:.word 0
t52:.word 0
t53:.word 0
t54:.word 0
t55:.word 0
t56:.word 0
t57:.word 0
t58:.word 0
t59:.word 0
t60:.word 0
t61:.word 0
t62:.word 0
t63:.word 0
t64:.word 0
t65:.word 0
t66:.word 0
t67:.word 0
t68:.word 0
t69:.word 0
t70:.word 0
t71:.word 0
t72:.word 0
t73:.word 0
t74:.word 0
t75:.word 0
t76:.word 0
t77:.word 0
t78:.word 0
t79:.word 0
t80:.word 0
t81:.word 0
t82:.word 0
t83:.word 0
t84:.word 0
t85:.word 0
t86:.word 0
t87:.word 0
t88:.word 0
t89:.word 0
t90:.word 0
t91:.word 0
t92:.word 0
t93:.word 0
t94:.word 0
t95:.word 0
t96:.word 0
t97:.word 0
t98:.word 0
t99:.word 0
t100:.word 0
t101:.word 0
t102:.word 0
t103:.word 0
t104:.word 0
t105:.word 0
t106:.word 0
t107:.word 0
t108:.word 0
t109:.word 0
t110:.word 0
t111:.word 0
t112:.word 0
t113:.word 0
t114:.word 0
t115:.word 0
t116:.word 0
t117:.word 0
t118:.word 0
t119:.word 0
t120:.word 0
t121:.word 0
t122:.word 0
t123:.word 0
t124:.word 0
t125:.word 0
t126:.word 0
t127:.word 0
t128:.word 0
t129:.word 0
t130:.word 0
t131:.word 0
t132:.word 0
t133:.word 0
t134:.word 0
t135:.word 0
t136:.word 0
t137:.word 0
t138:.word 0
t139:.word 0
t140:.word 0
t141:.word 0
t142:.word 0
t143:.word 0
t144:.word 0
t145:.word 0
t146:.word 0
t147:.word 0
t148:.word 0
t149:.word 0
t150:.word 0
t151:.word 0
t152:.word 0
t153:.word 0
t154:.word 0
t155:.word 0
t156:.word 0
t157:.word 0
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

main:
li $s0, 6
li $s1, 6
la $s4, t3
add $s3 $s0 $s1
sw $s3 0($s4)
li $s0, 11
li $s1, 1
la $s4, t4
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t3
lw $s0 0($s1)
la $s2, t4
lw $s1 0($s2)
la $s4, t2
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t2
lw $s0 0($s1)
la $s1, t1
sw $s0 0($s1)
li $s0, 7
li $s1, 4
la $s4, t10
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t10
lw $s0 0($s1)
la $s1, t9
sw $s0 0($s1)
li $s0, 3
li $s1, 4
la $s4, t17
add $s3 $s0 $s1
sw $s3 0($s4)
li $s0, 5
li $s1, 6
la $s4, t21
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t21
lw $s0 0($s1)
li $s1, 3
la $s4, t18
div $s0 $s1
mflo $s3
sw $s3 0($s4)
la $s1, t17
lw $s0 0($s1)
la $s2, t18
lw $s1 0($s2)
la $s4, t15
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t15
lw $s0 0($s1)
li $s1, 4
la $s4, t14
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t14
lw $s0 0($s1)
la $s1, t13
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t26
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t26
lw $s0 0($s1)
la $s1, t25
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t31
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t31
lw $s0 0($s1)
la $s2, t13
lw $s1 0($s2)
la $s4, t30
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t30
lw $s0 0($s1)
la $s1, t29
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t37
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t37
lw $s0 0($s1)
la $s2, t13
lw $s1 0($s2)
la $s4, t36
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t36
lw $s0 0($s1)
la $s1, t35
sw $s0 0($s1)
li $s0, 42
la $s1, t41
sw $s0 0($s1)
li $s0, 0
la $s1, t43
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t54
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t54
lw $s0 0($s1)
la $s2, t13
lw $s1 0($s2)
la $s4, t52
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t52
lw $s0 0($s1)
la $s2, t25
lw $s1 0($s2)
la $s4, t50
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t50
lw $s0 0($s1)
la $s2, t29
lw $s1 0($s2)
la $s4, t48
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t48
lw $s0 0($s1)
la $s2, t35
lw $s1 0($s2)
la $s4, t47
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t47
lw $s0 0($s1)
la $s1, t35
sw $s0 0($s1)
la $s1, t35
lw $s0 0($s1)
la $s1, t46
sw $s0 0($s1)
la $s1, t35
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
la $s1, t9
lw $s0 0($s1)
la $s2, t1
lw $s1 0($s2)
la $s4, t61
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t61
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
l1:
la $s1, t9
lw $s0 0($s1)
la $s2, t1
lw $s1 0($s2)
la $s4, t65
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t35
lw $s0 0($s1)
la $s2, t65
lw $s1 0($s2)
bgt $s0,$s1,l2
j l3
l2:
la $s1, t35
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t74
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t1
lw $s0 0($s1)
li $s1, 2
la $s4, t75
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t74
lw $s0 0($s1)
la $s2, t75
lw $s1 0($s2)
la $s4, t72
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t13
lw $s0 0($s1)
la $s2, t25
lw $s1 0($s2)
la $s4, t73
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t72
lw $s0 0($s1)
la $s2, t73
lw $s1 0($s2)
la $s4, t70
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t70
lw $s0 0($s1)
la $s2, t35
lw $s1 0($s2)
la $s4, t69
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t69
lw $s0 0($s1)
la $s1, t45
sw $s0 0($s1)
la $s1, t45
lw $s0 0($s1)
la $s1, t68
sw $s0 0($s1)
la $s1, t45
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
la $s1, t43
lw $s0 0($s1)
li $s1, 4
la $s4, t93
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t41
lw $s0 0($s1)
la $s2, t93
lw $s1 0($s2)
la $s4, t90
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t90
lw $s0 0($s1)
li $s1, 3
la $s4, t88
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t88
lw $s0 0($s1)
li $s1, 4
la $s4, t86
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t86
lw $s0 0($s1)
li $s1, 5
la $s4, t85
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t85
lw $s0 0($s1)
la $s1, t41
sw $s0 0($s1)
la $s1, t41
lw $s0 0($s1)
la $s1, t84
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
li $s1, 1
la $s4, t98
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t98
lw $s0 0($s1)
li $s1, 0
la $s4, t97
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t97
lw $s0 0($s1)
la $s1, t43
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
la $s1, t96
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
li $s1, 3
la $s4, t106
div $s0 $s1
mflo $s3
sw $s3 0($s4)
la $s1, t106
lw $s0 0($s1)
li $s1, 3
la $s4, t105
mul $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t43
lw $s0 0($s1)
la $s2, t105
lw $s1 0($s2)
la $s4, t102
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t1
lw $s0 0($s1)
la $s2, t1
lw $s1 0($s2)
la $s4, t112
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t112
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t110
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t110
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t103
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t102
lw $s0 0($s1)
la $s2, t103
lw $s1 0($s2)
beq $s0,$s1,l4
j l5
l4:
la $s1, t35
lw $s0 0($s1)
li $s1, 2
la $s4, t117
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t117
lw $s0 0($s1)
la $s1, t35
sw $s0 0($s1)
la $s1, t35
lw $s0 0($s1)
la $s1, t116
sw $s0 0($s1)
l5:
la $s1, t35
lw $s0 0($s1)
li $s1, 1
la $s4, t121
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t121
lw $s0 0($s1)
la $s1, t35
sw $s0 0($s1)
la $s1, t35
lw $s0 0($s1)
la $s1, t120
sw $s0 0($s1)
j l1
l3:
la $s1, t41
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
la $s1, t41
lw $s0 0($s1)
la $s1, t43
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
la $s1, t126
sw $s0 0($s1)
l6:
li $s0, 1200
li $s1, 22
la $s4, t129
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t41
lw $s0 0($s1)
la $s2, t129
lw $s1 0($s2)
bge $s0,$s1,l7
j l8
l7:
la $s1, t41
lw $s0 0($s1)
li $s1, 1024
la $s4, t133
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t133
lw $s0 0($s1)
la $s1, t43
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
la $s1, t132
sw $s0 0($s1)
la $s1, t41
lw $s0 0($s1)
li $s1, 1
la $s4, t137
sub $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t137
lw $s0 0($s1)
la $s1, t41
sw $s0 0($s1)
la $s1, t41
lw $s0 0($s1)
la $s1, t136
sw $s0 0($s1)
la $s1, t41
lw $s0 0($s1)
la $s1, t43
sw $s0 0($s1)
la $s1, t43
lw $s0 0($s1)
la $s1, t140
sw $s0 0($s1)
j l6
l8:
la $s1, t41
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t145
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t145
lw $s0 0($s1)
la $s1, t1
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s1, t144
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t149
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t149
lw $s0 0($s1)
la $s1, t9
sw $s0 0($s1)
la $s1, t9
lw $s0 0($s1)
la $s1, t148
sw $s0 0($s1)
la $s1, t1
lw $s0 0($s1)
la $s2, t9
lw $s1 0($s2)
la $s4, t153
add $s3 $s0 $s1
sw $s3 0($s4)
la $s1, t153
lw $s0 0($s1)
la $s1, t13
sw $s0 0($s1)
la $s1, t13
lw $s0 0($s1)
la $s1, t152
sw $s0 0($s1)
la $s1, t13
lw $a0 0($s1)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
li $v0, 0
jr $ra
