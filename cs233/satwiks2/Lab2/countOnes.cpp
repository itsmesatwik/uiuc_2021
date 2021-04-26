/**
 * @file
 * Contains an implementation of the countOnes function.
 */

unsigned countOnes(unsigned input) {
	// TODO: write your code here


    // 1-bit counters
    unsigned right_counters = input&(0x55555555);
    unsigned left_counters = input&(0xAAAAAAAA);

    left_counters = left_counters>>1;

    input = left_counters + right_counters;

    // 2-bit counters
    right_counters = input&(0x33333333);
    left_counters = input&(0xCCCCCCCC);

    left_counters = left_counters>>2;

    input = left_counters + right_counters;

    // 4-bit counters
    right_counters = input&(0x0F0F0F0F);
    left_counters = input&(0xF0F0F0F0);

    left_counters = left_counters>>4;

    input = left_counters + right_counters;

    // 8-bit counters
    right_counters = input&(0x00FF00FF);
    left_counters = input&(0xFF00FF00);

    left_counters = left_counters>>8;

    input = left_counters + right_counters;

    // 16-bit counters
    right_counters = input&(0x0000FFFF);
    left_counters = input&(0xFFFF0000);

    left_counters = left_counters>>16;

    input = left_counters + right_counters;

	return input;
}
