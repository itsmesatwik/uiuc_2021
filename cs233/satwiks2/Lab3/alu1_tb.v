module alu1_test;
    // exhaustively test your 1-bit ALU implementation by adapting mux4_tb.v
    reg A = 0;
    always #1 A = !A;
    reg B = 0;
    always #2 B = !B;
    reg carryin = 0;
    always #8 carryin = !carryin;

    reg[2:0] control = 0;

    initial begin
        $dumpfile("alu1.vcd");
        $dumpvars(0, alu1_test);

        # 16 control = 1;
        # 16 control = 2;
        # 16 control = 3;
        # 16 control = 4;
        # 16 control = 5;
        # 16 control = 6;
        # 16 control = 7;
        # 16 $finish;
    end

    wire out, carryout;
    alu1 a1(out, carryout, A, B, carryin, control);
    
endmodule
