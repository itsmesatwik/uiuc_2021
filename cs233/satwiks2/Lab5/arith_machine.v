// arith_machine: execute a series of arithmetic instructions from an instruction cache
//
// except (output) - set to 1 when an unrecognized instruction is to be executed.
// clock  (input)  - the clock signal
// reset  (input)  - set to 1 to set all registers to zero, set to 0 for normal execution.

module arith_machine(except, clock, reset);
    output      except;
    input       clock, reset;

    wire [31:0] inst;
    wire [31:0] PC, nextPC;
    // Start PC register with the initial value of 0

    alu32 al1(nextPC, , , , PC, 32'b100,3'b010);
    // DO NOT comment out or rename this module
    // or the test bench will break
    register #(32) PC_reg(PC, nextPC, clock, 1'b1, reset);
    wire [29:0] addr = PC[31:2];

    // DO NOT comment out or rename this module
    // or the test bench will break
    instruction_memory im(inst, addr);

    wire[5:0] opcode = inst[31:26];
    wire[5:0] funct = inst[5:0];
    wire[2:0] alu_op;
    wire writeenable, alu_src2, rd_src;

    mips_decode mips1(alu_op, writeenable, rd_src, alu_src2, except, opcode, funct);

    wire[4:0] Rs = inst[25:21];
    wire[4:0] Rt = inst[20:16];
    wire[4:0] Rd = inst[15:11];
    wire[4:0] Rdest;

    mux2v mx1(Rdest, Rd, Rt, rd_src);
    wire[31:0] rsData, rtData, rdData;

    // DO NOT comment out or rename this module
    // or the test bench will break
    regfile rf (rsData, rtData, Rs, Rt, Rdest, rdData, writeenable, clock, reset);
    wire msb = {16{inst[15]}};
    wire[15:0] b3 = inst[15:0];
    wire[31:0] imm32;
    assign imm32[31:0] = {{16{msb}}, b3};

    wire[31:0] b_alu;
    mux2v mx2(b_alu, rtData, imm32, alu_src2);
    alu32 al2(rdData, , , , rsData, b_alu, alu_op);

    /* add other modules */

endmodule // arith_machine
