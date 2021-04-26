.text

## bool
## board_done(unsigned short board[16][16]) {
##   for (int i = 0 ; i < 16 ; ++ i) {
##     for (int j = 0 ; j < 16 ; ++ j) {
##       if (!has_single_bit_set(board[i][j])) {
##         return false;
##       }
##     }
##   }
##   return true;
## }

.globl board_done
board_done:
	sub		$sp, $sp, 16	# set stack pointer

	sw		$ra, 0($sp)		#
	sw		$s0, 4($sp)		#
	sw		$s1, 8($sp)		#
	sw		$s2, 12($sp)		#


	move 	$s2, $a0		# $s2 = a0
	li		$s0, 0	# $s0 =
	li		$s1, 0		# $s1 =0

	row_loop:
	bge		$s0, 16, row_end	# if $s0 >16t row_end

	col_loop:
	bge		$s1, 16, col_end
	mul   $t1, $s0, 16
	add 	$t1, $t1, $s1
	mul   $t1, $t1, 2
	add  	$t1, $t1, $s2
	lhu 	$a0, 0($t1)
	jal		has_single_bit_set				# jump to has_single_bit_set and save position to $ra
	bne		$v0, 0, if_skip	# if $v0 !=  then if_skip
	li 		$v0, 0
	j		end_funct				# jump to end_funct

	if_skip:
	add		$s1, $s1, 1		# $s1 = s1 + 1
	j		col_loop				# jump to col_loop

	col_end:
	add $s0, $s0, 1
	li		$s1, 0		# $s1 =0
	j		row_loop				# jump to row_loop

	row_end:
	li		$v0, 1		# $v0 =1
	j		end_funct				# jump to end_funct


	end_funct:
	lw		$ra, 0($sp)		#
	lw		$s0, 4($sp)		#
	lw		$s1, 8($sp)		#
	lw		$s2, 12($sp)		#
	add		$sp, $sp, 16		#
	jr	$ra


## void
## print_board(unsigned short board[16][16]) {
##   for (int i = 0 ; i < 16 ; ++ i) {
##     for (int j = 0 ; j < 16 ; ++ j) {
##       int value = board[i][j];
##       char c = '*';
##       if (has_single_bit_set(value)) {
##         int num = get_lowest_set_bit(value) + 1;
##         c = symbollist[num];
##       }
##       putchar(c);
##     }
##     putchar('\n');
##   }
## }

.globl print_board
print_board:
	sub		$sp, $sp, 24	# set stack pointer

	sw		$ra, 0($sp)		#
	sw		$s0, 4($sp)		#
	sw		$s1, 8($sp)		#
	sw		$s2, 12($sp)		#
	sw		$s3, 16($sp)		#
	sw		$s4, 20($sp)		#




	move 	$s2, $a0		# $s2 = a0
	li		$s0, 0	# $s0 =
	li		$s1, 0		# $s1 =0

	row_loop_2:
	bge		$s0, 16, end_funct_2	# if $s0 >16t row_end

	col_loop_2:
	bge		$s1, 16, col_end_2

	mul   $s3, $s0, 16
	add 	$s3, $s3, $s1
	mul   $s3, $s3, 2
	add  	$s3, $s3, $s2

	lhu 	$a0, 0($s3)
	li		$s4, '*'		#$s4 ='*'

	jal		has_single_bit_set				# jump to has_single_bit_set and save position to $ra
	beq		$v0, 0, print_char_inner	# if $v0 ==  then if_skip


	lhu 	$a0, 0($s3)
	jal		get_lowest_set_bit				# jump to get_lowest_set_bit and save position to $ra
	la		$t1, symbollist		#

	add		$t0, $v0, 1		# $t0 = v0
	add		$t0, $t0, $t1		#  =  +0

	lb		$s4, 0($t0)		#
	j		print_char_inner				# jump to print_char_inner

	print_char_inner:
	move 	$a0, $s4		# $a0 = c
	li		$v0,  11		# $v0 = 11
	syscall
	add		$s1, $s1, 1		# $s1 = s1++

	j		col_loop_2				# jump to col_loop

	col_end_2:
	li 		$a0, '\n'		# $a0 = c
	li		$v0,  11		# $v0 = 11
	syscall
	add		$s0, $s0, 1
	li		$s1, 	0	# $s1 = 0
	j		row_loop_2				# jump to row_loop


	end_funct_2:
	lw		$ra, 0($sp)		#
	lw		$s0, 4($sp)		#
	lw		$s1, 8($sp)		#
	lw		$s2, 12($sp)		#
	lw		$s3, 16($sp)		#
	lw		$s4, 20($sp)		#
	add		$sp, $sp, 24		#

	jr	$ra
