  // full_machine: execute a series of MIPS instructions from an instruction cache
  //
  // except (output) - set to 1 when an unrecognized instruction is to be executed.
  // clock   (input) - the clock signal
  // reset   (input) - set to 1 to set all registers to zero, set to 0 for normal execution.

  module full_machine(except, clock, reset);
  output      except;
  input       clock, reset;

  wire [31:0] inst;
  wire [31:0] PC;

  wire mem_read, word_we, byte_we, byte_load, lui, slt, addm, zero, negative, overflow;
  wire[31:0]  branchOffset, muxin1, muxin2, nextPC;
  wire[1:0] control_type;
  wire[31:0] alu_out;
  wire[7:0] data_read5;
  wire[31:0] rsData, rtData, rdData, data_read2, data_read3, data_read4, data_out;
  wire[4:0] Rs = inst[25:21];
  wire[4:0] Rt = inst[20:16];
  wire[4:0] Rd = inst[15:11];
  wire[4:0] Rdest;
  wire [29:0] addr = PC[31:2];
  wire[5:0] opcode = inst[31:26];
  wire[5:0] funct = inst[5:0];
  wire[2:0] alu_op;
  wire writeenable, alu_src2, rd_src;
  wire msb = {16{inst[15]}};
  wire[15:0] b3 = inst[15:0];
  wire[31:0] imm32;
  wire[31:0] b_alu;
  alu32 muxal1(muxin1, , , , PC, 32'b100,3'b010);
  alu32 muxal2(muxin2, , , , muxin1, branchOffset,3'b010);

  mux4v mx4v1(nextPC, muxin1, muxin2, {{PC[31:28], inst[25:0]}, 2'b0},rsData, control_type);

  // DO NOT comment out or rename this module
  // or the test bench will break
  register #(32) PC_reg(PC, nextPC, clock, 1'b1, reset);


  // DO NOT comment out or rename this module
  // or the test bench will break
  instruction_memory im(inst, addr);



  mips_decode mips1(alu_op, writeenable, rd_src, alu_src2, except, control_type,
                     mem_read, word_we, byte_we, byte_load, lui, slt, addm, opcode, funct, zero);



  mux2v #(5) mx1(Rdest, Rd, Rt, rd_src); //rdrt


  mux2v regmux1(rdData, data_read2, {inst[15:0], 16'b0}, lui);
  mux2v regmux2(data_read2,data_read4, data_read3, mem_read);
  mux2v regmux3(data_read3, data_out, {24'b0, data_read5}, byte_load);
  mux2v regmux4(data_read4, alu_out, {31'b0, (negative & ~overflow) | (~negative & overflow)}, slt);
  mux4v #(8) regmux5(data_read5, data_out[7:0], data_out[15:8], data_out[23:16], data_out[31:24], alu_out[1:0]);
  // DO NOT comment out or rename this module
  // or the test bench will break
  regfile rf (rsData, rtData, Rs, Rt, Rdest, rdData, writeenable, clock, reset);

  assign imm32[31:0] = {{16{msb}}, b3};

  assign branchOffset = {imm32[29:0], 2'b0};



  mux2v mx2(b_alu, rtData, imm32, alu_src2);
  //alu32(out, overflow, zero, negative, inA, inB, control)
  alu32 al2(alu_out, overflow,zero, negative, rsData, b_alu, alu_op);

//data_mem(data_out, addr, data_in, word_we, byte_we, clk, reset);
  data_mem  mem1(data_out, alu_out, rtData, word_we, byte_we, clock, reset);

  endmodule // full_machine
