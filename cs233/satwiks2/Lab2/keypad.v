module keypad(valid, number, a, b, c, d, e, f, g);
   output 	valid;
   output [3:0] number;
   input 	a, b, c, d, e, f, g;
   wire w1, w2, w3, w4;

   // Checking for validity
   and a1 (w1, a, g);
   and a2 (w2, c, g);
   nor n1 (w3, w1, w2);
   or o1 (w4, a, b, c, d, e, f, g);
   and v1 (valid, w3, w4);


   wire not_a, not_b, not_c, not_d, not_e, not_f, not_g;

   // Not all the inputs
   not n2 (not_a, a);
   not n3 (not_b, b);
   not n4 (not_c, c);
   not n5 (not_d, d);
   not n6 (not_e, e);
   not n7 (not_f, f);
   not n8 (not_g, g);


   // Making the most significant bit
   wire msb, msb_1, msb_2;
   and a_msb (msb_1, not_a, b, not_c, not_d, not_e, f, not_g);
   and a_msb2 (msb_2, not_a, not_b, c, not_d, not_e, f, not_g);
   or o_msb (msb, msb_1, msb_2);

   // Making number[2]
   wire num2, num2_1, num2_2, num2_3, num2_4;
   and n2_1 (num2_1, a, not_b, not_c, not_d, e, not_f, not_g);
   and n2_2 (num2_2, not_a, b, not_c, not_d, e, not_f, not_g);
   and n2_3 (num2_3, not_a, not_b, c, not_d, e, not_f, not_g);
   and n2_4 (num2_4, a, not_b, not_c, not_d, not_e, f, not_g);
   or o_n2 (num2, num2_1, num2_2, num2_3, num2_4);

   // Making number[1]
   wire num1, num1_1, num1_2, num1_3, num1_4;
   and n1_1 (num1_1, not_a, b, not_c, d, not_e, not_f, not_g);
   and n1_2 (num1_2, not_a, not_b, c, d, not_e, not_f, not_g);
   and n1_3 (num1_3, not_a, not_b, c, not_d, e, not_f, not_g);
   and n1_4 (num1_4, a, not_b, not_c, not_d, not_e, f, not_g);
   or o_n1 (num1, num1_1, num1_2, num1_3, num1_4);

   // Making number[0]
   wire num0, num0_1, num0_2, num0_3, num0_4, num0_5;
   and n0_1 (num0_1, a, not_b, not_c, d, not_e, not_f, not_g);
   and n0_2 (num0_2, not_a, b, not_c, not_d, e, not_f, not_g);
   and n0_3 (num0_3, a, not_b, not_c, not_d, not_e, f, not_g);
   and n0_4 (num0_4, not_a, not_b, c, not_d, not_e, f, not_g);
   and n0_5 (num0_5, not_a, not_b, c, d, not_e, not_f, not_g);
   or o_n0 (num0, num0_1, num0_2, num0_3, num0_4, num0_5);

   assign number[3] = msb;
   assign number[2] = num2;
   assign number[1] = num1;
   assign number[0] = num0;


endmodule // keypad
