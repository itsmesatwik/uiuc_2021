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

RIGHT_WALL_SENSOR   = 0xffff0054
PICK_TREASURE           = 0xffff00e0
TREASURE_MAP            = 0xffff0058
MAZE_MAP                = 0xffff0050

REQUEST_PUZZLE          = 0xffff00d0
SUBMIT_SOLUTION         = 0xffff00d4

BONK_INT_MASK           = 0x1000
BONK_ACK                = 0xffff0060

TIMER_INT_MASK          = 0x8000
TIMER_ACK               = 0xffff006c

REQUEST_PUZZLE_INT_MASK = 0x800
REQUEST_PUZZLE_ACK      = 0xffff00d8




# T4 -- MAP
# T5 -- BOT X
# T6 -- BOT Y

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
.data
#Insert whatever static memory you need here

.align 4
puzzle: .space 512

.align 4
treasure_map: .space 404

.align 4
map: .space 900

.text
main:
    li      $t0, 0
    sw      $t0, ANGLE($zero)
    li      $t0, 1
    sw      $t0, ANGLE_CONTROL($zero)
    la      $t0, map
    sw      $t0, MAZE_MAP($zero)
    la      $t4, treasure_map
    sw      $t4, TREASURE_MAP($zero)
#    # Insert code here
    li      $t0, REQUEST_PUZZLE_INT_MASK# turn dat puzzle int on
#    or      $t0, BONK_INT_MASK          # turn dat bonk int on
    or      $t0, $t0, 1
    mtc0    $t0, $12
    la      $t7, puzzle

    li      $t0, 0
mpuzlo:
    sw      $t7, REQUEST_PUZZLE($zero)
    add     $t0, $t0, 1
    blt     $t0, 8, mpuzlo
#
    li      $t0, 10                     # full speed ahead, b0ss
    sw      $t0, VELOCITY($zero)
    li      $t1, 1
    li      $s0, 1
mainlo:
    lw      $t5, BOT_X($zero)
    lw      $t6 , BOT_Y($zero)
    div     $t5, $t5, 10
    div     $t6, $t6, 10
    move    $a0, $t5
    move    $a1, $t6
    lw      $a2, 0($t4)
    add     $a3, $t4, 4
    jal     treasure_check
    beq     $v0, $zero, mnopick
    sw      $v0, PICK_TREASURE($zero)
    sw      $t4, TREASURE_MAP($zero)
mnopick:
    lw      $t0, RIGHT_WALL_SENSOR($zero)
    bne     $t0, $zero, mnochange       # if we have no wall and last time we had a wall, turn right
    add     $s0, $s0, 1
    blt     $s0, 4, mnochange
    sub     $s0, $s0, 4
mnochange:
mbonkcheck:
    move    $t1, $s0
    move    $a0, $s0
    jal     checkleft
    move    $s0, $v0
    bne     $t1, $s0, mbonkcheck

    move    $a0, $s0
    jal     movecell
    j       mainlo
    jr      $ra                         #ret
#
## $a0 botx, $a1 boty, $a2 len, $a3 *treasures
treasure_check:
    sub     $sp, $sp, 12
    sw      $s0, 0($sp)
    sw      $s1, 4($sp)
    sw      $s2, 8($sp)

    li      $s0, 0
    li      $v0, 0
    loop:
    bge     $s0, $a2, end_loop
    mul     $s1, $s0, 8
    add     $s1, $s1, $a3
    lhu     $s2, 0($s1)
    bne     $s2, $a0, true_skip
    lhu     $s2, 2($s1)
    bne     $s2, $a1, true_skip
    li      $v0, 1
    j       end_loop
    true_skip:
    add     $s0, $s0, 1
    j       loop
    end_loop:

    lw      $s0, 0($sp)
    lw      $s1, 4($sp)
    lw      $s2, 8($sp)
    add     $sp, $sp, 12

    jr      $ra

checkleft:
    sub     $sp, $sp, 16
    sw      $ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    li      $s0, 0
    li      $s1, 1
    li      $s2, -90
    beq     $a0, 0, cln
    beq     $a0, 1, cle
    beq     $a0, 2, cls
    beq     $a0, 3, clw
cle:
    add     $s0, 90
cln:
    add     $s0, 90
clw:
    add     $s0, 90
cls:
    add     $s0, 90
    
    add     $s0, $s0, -90
    sw      $s0, ANGLE($zero)
    sw      $s1, ANGLE_CONTROL

    lw      $s2, RIGHT_WALL_SENSOR
    beq     $s2, $zero, clnobonk
    add     $a0, $a0, -1
    bge     $a0, $zero, clnobonk
    add     $a0, $a0, 4
clnobonk:
    move    $v0, $a0
    lw      $s2, 12($sp)
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, 0($sp)
    add     $sp, $sp, 16
    jr      $ra

movecell:
    sub     $sp, $sp, 4
    sw      $ra, 0($sp)
    la      $ra, mcend
    beq     $a0, 0, move_north
    beq     $a0, 1, move_east
    beq     $a0, 2, move_south
    beq     $a0, 3, move_west
mcend:
    lw      $ra, 0($sp)
    add     $sp, $sp, 4
    jr      $ra

#=====================================
move_north:
    sub     $sp, $sp, 16
    sw      $ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    lw      $s0, BOT_Y($zero)
    add     $s1, $s0, -10

    li      $s2, 270
    sw      $s2, ANGLE($zero)
    li      $s2, 1
    sw      $s2, ANGLE_CONTROL($zero)
    li      $s2, 10
    sw      $s2, VELOCITY($zero)

mnlo:
    li      $s2, 10
    sw      $s2, VELOCITY($zero)
    lw      $s0, BOT_Y($zero)
    bgt     $s0, $s1, mnlo

    li      $s2, 0
    sw      $s2, VELOCITY($zero)

    lw      $s2, 12($sp)
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, 0($sp)
    add     $sp, $sp, 16
    jr      $ra

move_east:
    sub     $sp, $sp, 16
    sw      $ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    lw      $s0, BOT_X($zero)
    add     $s1, $s0, 10

    li      $s2, 0
    sw      $s2, ANGLE($zero)
    li      $s2, 1
    sw      $s2, ANGLE_CONTROL($zero)
    li      $s2, 10
    sw      $s2, VELOCITY($zero)

melo:
    li      $s2, 10
    sw      $s2, VELOCITY($zero)
    lw      $s0, BOT_X($zero)
    blt     $s0, $s1, melo

    li      $s2, 0
    sw      $s2, VELOCITY($zero)

    lw      $s2, 12($sp)
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, 0($sp)
    add     $sp, $sp, 16
    jr      $ra

move_south:
    sub     $sp, $sp, 16
    sw      $ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    lw      $s0, BOT_Y($zero)
    add     $s1, $s0, 10

    li      $s2, 90
    sw      $s2, ANGLE($zero)
    li      $s2, 1
    sw      $s2, ANGLE_CONTROL($zero)
    li      $s2, 10
    sw      $s2, VELOCITY($zero)

mslo:
    li      $s2, 10
    sw      $s2, VELOCITY($zero)
    lw      $s0, BOT_Y($zero)
    blt     $s0, $s1, mslo

    li      $s2, 0
    sw      $s2, VELOCITY($zero)

    lw      $s2, 12($sp)
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, 0($sp)
    add     $sp, $sp, 16
    jr      $ra

move_west:
    sub     $sp, $sp, 16
    sw      $ra, 0($sp)
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)
    lw      $s0, BOT_X($zero)
    add     $s1, $s0, -10

    li      $s2, 180
    sw      $s2, ANGLE($zero)
    li      $s2, 1
    sw      $s2, ANGLE_CONTROL($zero)
    li      $s2, 10
    sw      $s2, VELOCITY($zero)

mwlo:
    li      $s2, 10
    sw      $s2, VELOCITY($zero)
    lw      $s0, BOT_X($zero)
    bgt     $s0, $s1, mwlo

    li      $s2, 0
    sw      $s2, VELOCITY($zero)

    lw      $s2, 12($sp)
    lw      $s1, 8($sp)
    lw      $s0, 4($sp)
    lw      $ra, 0($sp)
    add     $sp, $sp, 16
    jr      $ra
#=====================================

## bool
## rule1(unsigned short board[GRID_SQUARED][GRID_SQUARED]) {
##   bool changed = false;
##   for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
##     for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
##       unsigned value = board[i][j];
##       if (has_single_bit_set(value)) {
##         for (int k = 0 ; k < GRID_SQUARED ; ++ k) {
##           // eliminate from row
##           if (k != j) {
##             if (board[i][k] & value) {
##               board[i][k] &= ~value;
##               changed = true;
##             }
##           }
##           // eliminate from column
##           if (k != i) {
##             if (board[k][j] & value) {
##               board[k][j] &= ~value;
##               changed = true;
##             }
##           }
##         }
## 
##         // elimnate from square
##         int ii = get_square_begin(i);
##         int jj = get_square_begin(j);
##         for (int k = ii ; k < ii + GRIDSIZE ; ++ k) {
##           for (int l = jj ; l < jj + GRIDSIZE ; ++ l) {
##             if ((k == i) && (l == j)) {
##               continue;
##             }
##             if (board[k][l] & value) {
##               board[k][l] &= ~value;
##               changed = true;
##             }
##           }
##         }
##       }
##     }
##   }
##   return changed;
## }

## I chose to make a helper function to compute board addresses.
board_address:
	mul	$v0, $a1, 16		# i*16
	add	$v0, $v0, $a2		# (i*16)+j
	sll	$v0, $v0, 1		# ((i*9)+j)*2
	add	$v0, $a0, $v0
	jr	$ra

.globl rule1
rule1:
	sub	$sp, $sp, 32 		
	sw	$ra, 0($sp)		# save $ra and free up 7 $s registers for
	sw	$s0, 4($sp)		# i
	sw	$s1, 8($sp)		# j
	sw	$s2, 12($sp)		# board
	sw	$s3, 16($sp)		# value
	sw	$s4, 20($sp)		# k
	sw	$s5, 24($sp)		# changed
	sw	$s6, 28($sp)		# temp
	move	$s2, $a0		# store the board base address
	li	$s5, 0			# changed = false

	li	$s0, 0			# i = 0
r1_loop1:
	li	$s1, 0			# j = 0
r1_loop2:
	move	$a0, $s2		# board
	move 	$a1, $s0		# i
	move	$a2, $s1		# j
	jal	board_address
	lhu	$s3, 0($v0)		# value = board[i][j]
	move	$a0, $s3		
	jal	has_single_bit_set
	beq	$v0, 0, r1_loop2_bot	# if not a singleton, we can go onto the next iteration

	li	$s4, 0			# k = 0
r1_loop3:
	beq	$s4, $s1, r1_skip_row	# skip if (k == j)
	move	$a0, $s2		# board
	move 	$a1, $s0		# i
	move	$a2, $s4		# k
	jal	board_address
	lhu	$t0, 0($v0)		# board[i][k]
	and	$t1, $t0, $s3		
	beq	$t1, 0, r1_skip_row
	not	$t1, $s3
	and	$t1, $t0, $t1		
	sh	$t1, 0($v0)		# board[i][k] = board[i][k] & ~value
	li	$s5, 1			# changed = true
	
r1_skip_row:
	beq	$s4, $s0, r1_skip_col	# skip if (k == i)
	move	$a0, $s2		# board
	move 	$a1, $s4		# k
	move	$a2, $s1		# j
	jal	board_address
	lhu	$t0, 0($v0)		# board[k][j]
	and	$t1, $t0, $s3		
	beq	$t1, 0, r1_skip_col
	not	$t1, $s3
	and	$t1, $t0, $t1		
	sh	$t1, 0($v0)		# board[k][j] = board[k][j] & ~value
	li	$s5, 1			# changed = true

r1_skip_col:	
	add	$s4, $s4, 1		# k ++
	blt	$s4, 16, r1_loop3

	## doubly nested loop
	move	$a0, $s0		# i
	jal	get_square_begin
	move	$s6, $v0		# ii
	move	$a0, $s1		# j
	jal	get_square_begin	# jj

	move 	$t0, $s6		# k = ii
	add	$t1, $t0, 4		# ii + GRIDSIZE
	add 	$s6, $v0, 4		# jj + GRIDSIZE

r1_loop4_outer:
	sub	$t2, $s6, 4		# l = jj  (= jj + GRIDSIZE - GRIDSIZE)

r1_loop4_inner:
	bne	$t0, $s0, r1_loop4_1
	beq	$t2, $s1, r1_loop4_bot

r1_loop4_1:	
	mul	$v0, $t0, 16		# k*16
	add	$v0, $v0, $t2		# (k*16)+l
	sll	$v0, $v0, 1		# ((k*16)+l)*2
	add	$v0, $s2, $v0		# &board[k][l]
	lhu	$v1, 0($v0)		# board[k][l]
   	and	$t3, $v1, $s3		# board[k][l] & value
	beq	$t3, 0, r1_loop4_bot

	not	$t3, $s3
	and	$v1, $v1, $t3		
	sh	$v1, 0($v0)		# board[k][l] = board[k][l] & ~value
	li	$s5, 1			# changed = true

r1_loop4_bot:	
	add	$t2, $t2, 1		# l++
	blt	$t2, $s6, r1_loop4_inner

	add	$t0, $t0, 1		# k++
	blt	$t0, $t1, r1_loop4_outer
	

r1_loop2_bot:	
	add	$s1, $s1, 1		# j ++
	blt	$s1, 16, r1_loop2

	add	$s0, $s0, 1		# i ++
	blt	$s0, 16, r1_loop1

	move	$v0, $s5		# return changed
	lw	$ra, 0($sp)		# restore registers and return
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	lw	$s4, 20($sp)
	lw	$s5, 24($sp)
	lw	$s6, 28($sp)
	add	$sp, $sp, 32
	jr	$ra

.globl get_square_begin
get_square_begin:
	div	$v0, $a0, 4
	mul	$v0, $v0, 4
	jr	$ra


.globl has_single_bit_set
has_single_bit_set:
	beq	$a0, 0, hsbs_ret_zero	# return 0 if value == 0
	sub	$a1, $a0, 1
	and	$a1, $a0, $a1
	bne	$a1, 0, hsbs_ret_zero	# return 0 if (value & (value - 1)) == 0
	li	$v0, 1
	jr	$ra
hsbs_ret_zero:
	li	$v0, 0
	jr	$ra


get_lowest_set_bit:
	li	$v0, 0			# i
	li	$t1, 1

glsb_loop:
	sll	$t2, $t1, $v0		# (1 << i)
	and	$t2, $t2, $a0		# (value & (1 << i))
	bne	$t2, $0, glsb_done
	add	$v0, $v0, 1
	blt	$v0, 16, glsb_loop	# repeat if (i < 16)

	li	$v0, 0			# return 0
glsb_done:
	jr	$ra


.globl print_board
print_board:
	sub	$sp, $sp, 20
	sw	$ra, 0($sp)		# save $ra and free up 4 $s registers for
	sw	$s0, 4($sp)		# i
	sw	$s1, 8($sp)		# j
	sw	$s2, 12($sp)		# the function argument
	sw	$s3, 16($sp)		# the computed pointer (which is used for 2 calls)
	move	$s2, $a0

	li	$s0, 0			# i
pb_loop1:
	li	$s1, 0			# j
pb_loop2:
	mul	$t0, $s0, 16		# i*16
	add	$t0, $t0, $s1		# (i*16)+j
	sll	$t0, $t0, 1		# ((i*16)+j)*2
	add	$s3, $s2, $t0
	lhu	$a0, 0($s3)
	jal	has_single_bit_set		
	beq	$v0, 0, pb_star		# if it has more than one bit set, jump
	lhu	$a0, 0($s3)
	jal	get_lowest_set_bit	# 
	add	$v0, $v0, 1		# $v0 = num
	la	$t0, symbollist
	add	$a0, $v0, $t0		# &symbollist[num]
	lb	$a0, 0($a0)		#  symbollist[num]
	li	$v0, 11
	syscall
	j	pb_cont

pb_star:		
	li	$v0, 11			# print a "*"
	li	$a0, '*'
	syscall

pb_cont:	
	add	$s1, $s1, 1		# j++
	blt	$s1, 16, pb_loop2

	li	$v0, 11			# at the end of a line, print a newline char.
	li	$a0, '\n'
	syscall	
	
	add	$s0, $s0, 1		# i++
	blt	$s0, 16, pb_loop1

	lw	$ra, 0($sp)		# restore registers and return
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	add	$sp, $sp, 20
	jr	$ra

# Kernel Text
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

        and     $a0, $k0, REQUEST_PUZZLE_INT_MASK
        bne     $a0, 0, request_puzzle_interrupt

        li        $v0, PRINT_STRING    # Unhandled interrupt types
        la        $a0, unhandled_str
        syscall
        j    done

bonk_interrupt:
        sw      $v0, BONK_ACK        # acknowledge interrupt
        j       interrupt_dispatch    # see if other interrupts are waiting

request_puzzle_interrupt:
        sw  $v0, REQUEST_PUZZLE_ACK     #acknowledge interrupt
        li      $t0, 0
        sw      $t0, VELOCITY($zero)
rp_int_solve:
        la      $a0, puzzle
        la      $ra, afterr1
        la      $t1, rule1
        jr      $t1
afterr1:
        bne     $v0, $zero, rp_int_solve
        la      $a0, puzzle
        sw      $a0, SUBMIT_SOLUTION($zero)

    j   interrupt_dispatch   # see if other interrupts are waiting

timer_interrupt:
        sw       $v0, TIMER_ACK        # acknowledge interrupt
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

