// output is A (when control == 0) or B (when control == 1)
module mux2(out, A, B, control);
    output out;
    input  A, B;
    input  control;
    wire   wA, wB, not_control;
         
    not n1(not_control, control);
    and a1(wA, A, not_control);
    and a2(wB, B, control);
    or  o1(out, wA, wB);
endmodule // mux2

// output is A (when control == 00) or B (when control == 01) or
//           C (when control == 10) or D (when control == 11)
module mux4(out, A, B, C, D, control);
    output      out;
    input       A, B, C, D;
    input [1:0] control;
    wire wA, wB, wC, wD, not_control0, not_control1;
    not n1(not_control1, control[1]);
    not n2(not_control0, control[0]);
    and a1(wA, A, not_control0, not_control1);
    and a2(wB, B, control[0], not_control1);
    and a3(wC, C, not_control0, control[1]);
    and a4(wD, D, control[0], control[1]);
    or o1(out, wA, wB, wC, wD);

endmodule // mux4
