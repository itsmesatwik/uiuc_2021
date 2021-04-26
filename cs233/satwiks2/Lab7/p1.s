.text

## bool has_single_bit_set(unsigned value) {  // returns 1 if a single bit is set
##   if (value == 0) {
##     return 0;   // has no bits set
##   }
##   if (value & (value - 1)) {
##     return 0;   // has more than one bit set
##   }
##   return 1;
## }

.globl has_single_bit_set
has_single_bit_set:
    li   $t0  0
    #li   $t1  1
    beq  $a0  $t0  no_bits
    sub  $t2  $a0  1    #$t2 = value-1
    and  $t2  $t2  $a0    #$t2 = value&(value-1)
    bne  $t2  $t0  no_bits
    li  $v0  1
    jr  $ra
    no_bits: li  $v0  0
	  jr	$ra



## unsigned get_lowest_set_bit(unsigned value) {
##   for (int i = 0 ; i < 16 ; ++ i) {
##     if (value & (1 << i)) {          # test if the i'th bit position is set
##       return i;                      # if so, return i
##     }
##   }
##   return 0;
## }

.globl get_lowest_set_bit
get_lowest_set_bit:
    li  $v0  0
    li  $t1  1
    loop:
        slti $t2  $v0  16
        beq  $t2  0   end_loop
        sllv $t3  $t1  $v0
        and  $t3  $t3  $a0
        bne  $t3  0   if_entered
        addi $v0  $v0  1
        j loop
    end_loop:
    li  $v0  0
	jr	$ra

    if_entered:
    jr $ra
