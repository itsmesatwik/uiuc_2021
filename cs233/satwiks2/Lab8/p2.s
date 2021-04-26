.text

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

.globl rule1
rule1:

    sub $sp $sp 28
    sw  $ra 0($sp)
    sw  $s0 4($sp)
    sw  $s1 8($sp)
    sw  $s2 12($sp)
    sw  $s4 16($sp)
    sw  $s5 20($sp)
    sw  $s6 24($sp)


    li  $s0 0   #changed = false
    li  $s1 0   #i = 0
    li  $s2 0   #j = 0

    move $s4 $a0 #storing board


    i_loop:
    bge $s1 16 i_loop_skip
    j_loop:
    bge $s2 16 i_loop_end

    mul $t0 $s1 16
    add $t0 $t0 $s2
    mul $t0 $t0 2
    add $t0 $t0 $s4

    lhu  $a0 0($t0)
    move $s5 $a0      # saving the value on stack for later use
    jal has_single_bit_set
    blt $v0 1 j_loop_end
    li  $t7 0   #k = 0
    k_loop:
    bge $t7 16 k_1_end
    beq $t7 $s2 if_1_end

    mul $t0 $s1 16
    add $t0 $t0 $t7
    mul $t0 $t0 2
    add $t0 $t0 $s4

    lhu  $t1 0($t0)
    and $t2 $t1 $s5
    blt $t2 1 if_1_end
    nor $t2 $s5 $s5
    and $t2 $t2 $t1
    sh  $t2 0($t0)

    li  $s0 1
    j   if_1_end

    if_1_end:
    beq $t7 $s1 if_2_end

    mul $t0 $t7 16
    add $t0 $t0 $s2
    mul $t0 $t0 2
    add $t0 $t0 $s4

    lhu  $t1 0($t0)
    and $t2 $t1 $s5
    blt $t2 1 if_2_end
    nor $t2 $s5 $s5
    and $t2 $t2 $t1
    sh  $t2 0($t0)
    li  $s0 1
    j   if_2_end

    if_2_end:
    add $t7 $t7 1
    j   k_loop

    k_1_end:
    move $a0 $s1
    jal  get_square_begin
    move $s6 $v0
    move $a0 $s2
    jal  get_square_begin
    move $t5 $v0
    add $t0 $s6 4
    add $t1 $v0 4

    k_loop_2:
    bge $s6 $t0 k_2_loop_skip

    l_loop:
    bge $v0 $t1 k_2_loop_end
    subu $t2 $s6 $s1
    subu $t3 $v0 $s2
    or  $t2 $t2 $t3
    beq $t2 $zero l_loop_end

    mul $t2 $s6 16
    add $t2 $t2 $v0
    mul $t2 $t2 2
    add $t2 $t2 $s4

    lhu $t3 0($t2)

    and $t4 $t3 $s5
    blt $t4  1  l_loop_end
    nor $t4 $s5 $s5
    and $t4 $t4 $t3
    sh  $t4 0($t2)
    li  $s0  1
    j   l_loop_end

    l_loop_end:
    add $v0 $v0 1
    j   l_loop

    k_2_loop_end:
    move $v0 $t5
    add  $s6 $s6 1
    j    k_loop_2

    k_2_loop_skip:
    j   j_loop_end

    j_loop_end:
    add $s2 $s2 1
    j   j_loop

    i_loop_end:
    li  $s2 0
    add $s1 $s1 1
    j   i_loop

    i_loop_skip:
    move $v0 $s0
    lw  $ra 0($sp)
    lw  $s0 4($sp)
    lw  $s1 8($sp)
    lw  $s2 12($sp)
    lw  $s4 16($sp)
    lw  $s5 20($sp)
    lw  $s6 24($sp)
    add $sp $sp 28

    jr	$ra
