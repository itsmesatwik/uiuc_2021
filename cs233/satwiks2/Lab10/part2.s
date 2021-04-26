.data
# syscall constants
PRINT_STRING            = 4
PRINT_CHAR              = 11
PRINT_INT               = 1

# memory-mapped I/O
VELOCITY                = 0xffff0010
ANGLE                   = 0xffff0014
ANGLE_CONTROL           = 0xffff0018

BOT_X                   = 0xffff0020
BOT_Y                   = 0xffff0024

TIMER                   = 0xffff001c

RIGHT_WALL_SENSOR 		  = 0xffff0054
PICK_TREASURE           = 0xffff00e0
TREASURE_MAP            = 0xffff0058

REQUEST_PUZZLE          = 0xffff00d0
SUBMIT_SOLUTION         = 0xffff00d4

BONK_INT_MASK           = 0x1000
BONK_ACK                = 0xffff0060

TIMER_INT_MASK          = 0x8000
TIMER_ACK               = 0xffff006c

REQUEST_PUZZLE_INT_MASK = 0x800
REQUEST_PUZZLE_ACK      = 0xffff00d8
# struct spim_treasure
#{
#    short x;
#    short y;
#    int points;
#};
#
#struct spim_treasure_map
#{
#    unsigned length;
#    struct spim_treasure treasures[50];
#};
#

.data
#REQUEST_PUZZLE returns an int array of length 128



.align 4
dfs:      .word 128
tree_arr: .space 512
#
#Put any other static memory you need here
#

.text
main:
    li      $a0, 0
    sw      $a0, VELOCITY($zero)
    li      $t4, TIMER_INT_MASK
    # or      $t4, $t4, BONK_INT_MASK
    or      $t4, $t4, REQUEST_PUZZLE_INT_MASK
    or      $t4, $t4, 1
    mtc0    $t4, $12

    jal     gather_keys
    sw      $a0, PICK_TREASURE($zero)


    jal     move_up
    jal     move_right
    jal     move_down
    sw      $a0, PICK_TREASURE($zero)
    jal     move_right
    jal     move_down
    jal     move_left
    sw      $a0, PICK_TREASURE($zero)
    jal     move_down
    jal     move_left
    jal     move_up
    sw      $a0, PICK_TREASURE($zero)
    jal     move_left
    jal     move_up
    jal     move_up
    jal     move_up
    jal     move_right
    jal     move_right
    jal     move_right
    jal     move_up
    jal     move_up
    jal     move_right
    jal     move_right
    jal     move_right
    jal     move_right
    jal     move_right
    sw      $a0, PICK_TREASURE($zero)


    jal     move_up
    jal     move_right
    jal     move_down
    sw      $a0, PICK_TREASURE($zero)
    jal     move_right
    jal     move_down
    jal     move_left
    sw      $a0, PICK_TREASURE($zero)
    jal     move_down
    jal     move_left
    jal     move_up
    sw      $a0, PICK_TREASURE($zero)



  interrupt_delay:
    li      $t7, 0
    loop:
    beq     $t7, 70000, end_loop
    add     $t7, $t7, 1
    j       loop
    end_loop:
    jr      $ra                    #ret

gather_keys:
    sub     $sp, $sp, 4
    sw      $ra, 0($sp)

    li      $t8, 0
    key_loop:
    beq     $t8, 8, key_end
    la      $t5, tree_arr
    sw      $t5, REQUEST_PUZZLE($zero)
    jal     interrupt_delay

    la      $a0, tree_arr
    li      $a1, 1
    li      $a2, 1
    jal     dfs_
    la      $a0, dfs
    sw      $v0, 0($a0)
    sw      $a0, SUBMIT_SOLUTION($zero)
    add     $t8, $t8, 1
    j       key_loop
    key_end:
    lw      $ra, 0($sp)
    add     $sp, $sp, 4
    jr      $ra

dfs_:
		sub		$sp, $sp, 16		# STACK STORE
		sw 		$ra, 0($sp)		# Store ra
		sw		$s0, 4($sp)		# s0 = tree
		sw		$s1, 8($sp)		# s1 = i
		sw		$s2, 12($sp)	# s2 = input
		move 	$s0, $a0
		move 	$s1, $a1
		move	$s2, $a2


##	if (i >= 127) {
##		return -1;
##	}

_dfs_base_case_one:
    blt     $s1, 127, _dfs_base_case_two
    li      $v0, -1
    j _dfs_return


##	if (input == tree[i]) {
##		return 0;
##	}

_dfs_base_case_two:

		mul		$t1, $s1, 4
		add		$t2, $s0, $t1
    lw      $t1, 0($t2)  			# tree[i]

    bne     $t1, $s2, _dfs_ret_one
    li      $v0, 0
		j _dfs_return

##	int ret = DFS(tree, 2 * i, input);
##	if (ret >= 0) {
##		return ret + 1;
##	}
_dfs_ret_one:
		mul		$a1, $s1, 2
		jal 	dfs_				##	int ret = DFS(tree, 2 * i, input);


		blt		$v0, 0, _dfs_ret_two	##	if (ret >= 0)

		addi	$v0, 1					##	return ret + 1
		j _dfs_return

##	ret = DFS(tree, 2 * i + 1, input);
##	if (ret >= 0) {
##		return ret + 1;
##	}
_dfs_ret_two:
    mul		$a1, $s1, 2
		addi	$a1, 1
		jal 	dfs_				##	int ret = DFS(tree, 2 * i + 1, input);


		blt		$v0, 0, _dfs_return		##	if (ret >= 0)

		addi	$v0, 1					##	return ret + 1
		j _dfs_return

##	return ret;
_dfs_return:
		lw 		$ra, 0($sp)
		lw		$s0, 4($sp)
		lw		$s1, 8($sp)
		lw		$s2, 12($sp)
		add		$sp, $sp, 16
    jal     $ra


move_right:
    sub   $sp, $sp, 16
    sw    $ra, 0($sp)
    sw    $s0, 4($sp)
    sw    $s1, 8($sp)
    sw    $s2, 12($sp)
    lw    $s1, BOT_X($zero)
    add   $s2, $s1, 10

    li    $s0, 0
    sw    $s0, ANGLE($zero)
    li    $s0, 1
    sw    $s0, ANGLE_CONTROL($zero)
    sw    $s0, VELOCITY($zero)

right_loop:
    lw    $s1, BOT_X($zero)
    beq   $s1, $s2, right_loop_exit
    j     right_loop
right_loop_exit:
    li    $s0, 0
    sw    $s0, VELOCITY($zero)
    lw    $ra, 0($sp)
    lw    $s0, 4($sp)
    lw    $s1, 8($sp)
    lw    $s2, 12($sp)
    add   $sp, $sp, 16
    jr    $ra


move_left:
    sub   $sp, $sp, 16
    sw    $ra, 0($sp)
    sw    $s0, 4($sp)
    sw    $s1, 8($sp)
    sw    $s2, 12($sp)
    lw    $s1, BOT_X($zero)
    add   $s2, $s1, -10

    li    $s0, 180
    sw    $s0, ANGLE($zero)
    li    $s0, 1
    sw    $s0, ANGLE_CONTROL($zero)
    sw    $s0, VELOCITY($zero)

    left_loop:
    lw    $s1, BOT_X($zero)
    beq   $s1, $s2, left_loop_exit
    j     left_loop
    left_loop_exit:
    li    $s0, 0
    sw    $s0, VELOCITY($zero)
    lw    $ra, 0($sp)
    lw    $s0, 4($sp)
    lw    $s1, 8($sp)
    lw    $s2, 12($sp)
    add   $sp, $sp, 16
    jr    $ra
move_up:
    sub   $sp, $sp, 16
    sw    $ra, 0($sp)
    sw    $s0, 4($sp)
    sw    $s1, 8($sp)
    sw    $s2, 12($sp)
    lw    $s1, BOT_Y($zero)
    add   $s2, $s1, -10

    li    $s0, 270
    sw    $s0, ANGLE($zero)
    li    $s0, 1
    sw    $s0, ANGLE_CONTROL($zero)
    sw    $s0, VELOCITY($zero)

    up_loop:
    lw    $s1, BOT_Y($zero)
    beq   $s1, $s2, up_loop_exit
    j     up_loop
    up_loop_exit:
    li    $s0, 0
    sw    $s0, VELOCITY($zero)
    lw    $ra, 0($sp)
    lw    $s0, 4($sp)
    lw    $s1, 8($sp)
    lw    $s2, 12($sp)
    add   $sp, $sp, 16
    jr    $ra
move_down:
    sub   $sp, $sp, 16
    sw    $ra, 0($sp)
    sw    $s0, 4($sp)
    sw    $s1, 8($sp)
    sw    $s2, 12($sp)
    lw    $s1, BOT_Y($zero)
    add   $s2, $s1, 10

    li    $s0, 90
    sw    $s0, ANGLE($zero)
    li    $s0, 1
    sw    $s0, ANGLE_CONTROL($zero)
    sw    $s0, VELOCITY($zero)

    down_loop:
    lw    $s1, BOT_Y($zero)
    beq   $s1, $s2, down_loop_exit
    j     down_loop
    down_loop_exit:
    li    $s0, 0
    sw    $s0, VELOCITY($zero)
    lw    $ra, 0($sp)
    lw    $s0, 4($sp)
    lw    $s1, 8($sp)
    lw    $s2, 12($sp)
    add   $sp, $sp, 16
    jr    $ra

.kdata
chunkIH:    .space 28
non_intrpt_str:    .asciiz "Non-interrupt exception\n"
unhandled_str:    .asciiz "Unhandled interrupt type\n"
.ktext 0x80000180
interrupt_handler:
.set noat
        move      $k1, $at        # Save $at
.set at
        la        $k0, chunkIH
        sw        $a0, 0($k0)        # Get some free registers
        sw        $v0, 4($k0)        # by storing them to a global variable
        sw        $t0, 8($k0)
        sw        $t1, 12($k0)
        sw        $t2, 16($k0)
        sw        $t3, 20($k0)

        mfc0      $k0, $13             # Get Cause register
        srl       $a0, $k0, 2
        and       $a0, $a0, 0xf        # ExcCode field
        bne       $a0, 0, non_intrpt



interrupt_dispatch:            # Interrupt:
    mfc0       $k0, $13        # Get Cause register, again
    beq        $k0, 0, done        # handled all outstanding interrupts

    and        $a0, $k0, BONK_INT_MASK    # is there a bonk interrupt?
    bne        $a0, 0, bonk_interrupt

    and        $a0, $k0, TIMER_INT_MASK    # is there a timer interrupt?
    bne        $a0, 0, timer_interrupt

  	and 	$a0, $k0, REQUEST_PUZZLE_INT_MASK
  	bne 	$a0, 0, request_puzzle_interrupt

    li        $v0, PRINT_STRING    # Unhandled interrupt types
    la        $a0, unhandled_str
    syscall
    j    done

bonk_interrupt:
    sw       $a1, BONK_ACK($zero)

    li       $a1, 10
    li       $t0, 180
    lw       $a0, TIMER($zero)
    and      $a0, $a0, 1
    bne      $a0, $zero, bonk_skip
    li       $a1, -10   # see if other interrupts are waiting
    sw       $t0, ANGLE($zero)
    sw       $zero, ANGLE_CONTROL($zero)

bonk_skip:
    sw       $a1, VELOCITY($zero)
    j        interrupt_dispatch

request_puzzle_interrupt:
    sw         $a1, REQUEST_PUZZLE_ACK($zero)
  	#Fill in your code here
  	j	interrupt_dispatch

timer_interrupt:
    #Fill in your code here
    sw       $a1, TIMER_ACK($zero)

    j        interrupt_dispatch    # see if other interrupts are waiting

non_intrpt:                # was some non-interrupt
    li        $v0, PRINT_STRING
    la        $a0, non_intrpt_str
    syscall                # print out an error message
    # fall through to done

done:
    la      $k0, chunkIH
    lw      $a0, 0($k0)        # Restore saved registers
    lw      $v0, 4($k0)
	  lw      $t0, 8($k0)
    lw      $t1, 12($k0)
    lw      $t2, 16($k0)
    lw      $t3, 20($k0)
.set noat
    move    $at, $k1        # Restore $at
.set at
    eret
