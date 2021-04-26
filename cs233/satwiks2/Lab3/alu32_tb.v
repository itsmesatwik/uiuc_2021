//implement a test bench for your 32-bit ALU
module alu32_test;
    reg [31:0] A = 0, B = 0;
    reg [2:0] control = 0;

    initial begin
        $dumpfile("alu32.vcd");
        $dumpvars(0, alu32_test);

             A = 8; B = 4; control = `ALU_ADD; // try adding 8 and 4
        # 10 A = 2; B = 5; control = `ALU_SUB; // try subtracting 5 from 2
        # 10 A = 2; B = 2; control = `ALU_SUB; // ZERO
        # 10 A = 32'h7FFFFFFF; B = 1; control = `ALU_ADD; // overflow add
        # 10 A = 32'h80000000; B = 1; control = `ALU_SUB; //overflow sub
        # 10 A = 1; B = 0; control = `ALU_AND; //and with zero
        # 10 A = 1; B = 1; control = `ALU_AND; //and with one

        # 10 A = 1; B = 1; control = `ALU_OR; //or
        # 10 A = 1; B = 0; control = `ALU_OR; //or
        # 10 A = 0; B = 1; control = `ALU_OR; //or
        # 10 A = 0; B = 0; control = `ALU_OR; //or

        # 10 A = 1; B = 0; control = `ALU_NOR; //nor
        # 10 A = 0; B = 1; control = `ALU_NOR; //nor
        # 10 A = 1; B = 1; control = `ALU_NOR; //nor
        # 10 A = 0; B = 0; control = `ALU_NOR; //nor

        # 10 A = 1; B = 1; control = `ALU_XOR; //xor
        # 10 A = 0; B = 1; control = `ALU_XOR; //xor
        # 10 A = 1; B = 0; control = `ALU_XOR; //xor
        # 10 A = 0; B = 0; control = `ALU_XOR; //xor
        // add more test cases here!

        # 10 $finish;
    end

    wire [31:0] out;
    wire overflow, zero, negative;
    alu32 a(out, overflow, zero, negative, A, B, control);  
endmodule // alu32_test
