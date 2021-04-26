module logicunit_test;
    // exhaustively test your logic unit implementation by adapting mux4_tb.v
    reg A = 0;
    always #1 A = !A;
    reg B = 0;
    always #2 B = !B;

    reg[1:0] control = 0;

    initial begin
        $dumpfile("logicunit.vcd");
        $dumpvars(0, logicunit_test);


        # 16 control = 1;
        # 16 control = 2;
        # 16 control = 3;
        # 16 $finish;
    end
    
    wire out;
    
    logicunit l5(out, A, B, control);


endmodule // logicunit_test
