.text

##int
##dfs(int* tree, int i, int input) {
##	if (i >= 127) {
##		return -1;
##	}
##	if (input == tree[i]) {
##		return 0;
##	}
##
##	int ret = DFS(tree, 2 * i, input);
##	if (ret >= 0) {
##		return ret + 1;
##	}
##	ret = DFS(tree, 2 * i + 1, input);
##	if (ret >= 0) {
##		return ret + 1;
##	}
##	return ret;
##}

.globl dfs
dfs:
	#li		$t0, 127	# $t0 = 127
	move 	$t1, $a0		# $t1 = a0
	bge		$a1, 127, g_127
	mul   $t2, $a1, 4
	add		$t1, $t1, $t2		#
	lw		$t2, 0($t1)		#
	beq		$a2, $t2, inp_eq_tr
	sub		$sp, $sp, 8		# $sp = sp
	sw		$ra, 0($sp)		#
	sw		$s0, 4($sp)		#
	move 	$s0, $a1		# $s0 =

	add		$a1, $s0, $s0		# $a2 = a2
	jal		dfs				# jump to dfs
	move 	$a1, $s0		#  =
	bge		$v0, 0, ret_g_0	# if $v0 >=  then
	add		$a1, $s0, $s0		# a1  +
	add		$a1, $a1, 1		# $a1 = a+
	jal		dfs				# jump to dfs and save position to $ra
	bge		$v0, 0, ret_g_0
	lw		$ra, 0($sp)		#
	lw		$s0, 4($sp)
	add		$sp, $sp, 8		# $sp =  +
	jr		$ra					# jump to

	g_127:
	li		$v0, 	-1	# $v0 = -1
	jr		$ra					# jump to

	inp_eq_tr:
	li		$v0, 	0	# $v0 = 0
	jr		$ra					# jump to

	ret_g_0:
	add		$v0, $v0, 1		# $v0 = v0
	lw		$ra, 0($sp)		#
	lw		$s0, 4($sp)		#

	add		$sp, $sp, 8		# $sp =  +
	jr		$ra
