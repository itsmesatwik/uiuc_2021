module pipelined_machine(clk, reset);
    input        clk, reset;

    wire [31:0]  PC;
    wire [31:2]  next_PC, PC_plus4, PC_target, PC_plus4_pipe;
    wire [31:0]  inst, inst_pipe;

    wire [31:0]  imm = {{ 16{inst[15]} }, inst[15:0] };  // sign-extended immediate
    wire [4:0]   rs = inst[25:21];
    wire [4:0]   rt = inst[20:16];
    wire [4:0]   rd = inst[15:11];
    wire [5:0]   opcode = inst[31:26];
    wire [5:0]   funct = inst[5:0];

    wire [4:0]   wr_regnum;
    wire [4:0]   P_wr_regnum;
    wire [2:0]   ALUOp, P_ALUOp;

    wire         RegWrite, BEQ, ALUSrc, MemRead, MemWrite, MemToReg, RegDst;
    wire         PCSrc, zero;


    wire         P_RegWrite, P_ALUSrc, P_MemRead, P_MemWrite, P_MemToReg, P_RegDst;

    wire [31:0]  rd1_data, rd2_data, B_data, alu_out_data, load_data, wr_data;
    wire [31:0]  P_alu_out_data, P_rd2_data;
    wire [31:0]  Forward_rd1_data, Forward_rd2_data;

    wire         ForwardA, ForwardB;

    wire         stall;



    assign stall = (P_MemToReg == 1'b1) & (P_MemRead == 1'b1) & ((rs == P_wr_regnum) || (rt == P_wr_regnum));

    assign ForwardA = (rs == P_wr_regnum) & (P_RegWrite == 1'b1) & (rs != 5'b0);
    assign ForwardB = (rt == P_wr_regnum) & (P_RegWrite == 1'b1) & (rt != 5'b0);
    // DO NOT comment out or rename this module
    // or the test bench will break
    register #(30, 30'h100000) PC_reg(PC[31:2], next_PC[31:2], clk, ~stall, reset);

    assign PC[1:0] = 2'b0;  // bottom bits hard coded to 00
    adder30 next_PC_adder(PC_plus4, PC[31:2], 30'h1);

    // PIPELINE PC
    register #(30) Pipeline_PC(PC_plus4_pipe, PC_plus4, clk, ~stall, reset|PCSrc);
    adder30 target_PC_adder(PC_target, PC_plus4_pipe, imm[29:0]);
    mux2v #(30) branch_mux(next_PC, PC_plus4, PC_target, PCSrc);
    assign PCSrc = BEQ & zero;

    // DO NOT comment out or rename this module
    // or the test bench will break
    instruction_memory imem(inst_pipe, PC[31:2]);  //inst_pipe is the wire before pipelining for sake of not renaming inst at rs rt

    // PIPELINE INSTR
    register #(32) Pipiline_Instr(inst, inst_pipe, clk, ~stall, reset|PCSrc);

    mips_decode decode(ALUOp, RegWrite, BEQ, ALUSrc, MemRead, MemWrite, MemToReg, RegDst, opcode, funct);

    // PIPELINE DECODER
    register #(1) Pipeline_RegWrite (P_RegWrite, RegWrite, clk, 1'b1, stall|reset|PCSrc);
    register #(1) Pipeline_ALUSrc (P_ALUSrc, ALUSrc, clk, 1'b1, reset);
    register #(1) Pipeline_MemRead (P_MemRead, MemRead, clk, 1'b1, stall|reset|PCSrc);
    register #(1) Pipeline_MemWrite (P_MemWrite, MemWrite, clk, 1'b1, stall|reset|PCSrc);
    register #(1) Pipeline_MemToReg (P_MemToReg, MemToReg, clk, 1'b1, stall|reset|PCSrc);
    register #(1) Pipeline_RegDst (P_RegDst, RegDst, clk, 1'b1, reset);
    register #(3) Pipeline_ALUOp (P_ALUOp, ALUOp, clk, 1'b1, reset);

    // DO NOT comment out or rename this module
    // or the test bench will break
    regfile rf (rd1_data, rd2_data,
               rs, rt, P_wr_regnum, wr_data,
               P_RegWrite, clk, reset);

    mux2v #(32) ForwardA_mux (Forward_rd1_data, rd1_data, P_alu_out_data, ForwardA);
    mux2v #(32) ForwardB_mux (Forward_rd2_data, rd2_data, P_alu_out_data, ForwardB);

    mux2v #(32) imm_mux(B_data, Forward_rd2_data, imm, ALUSrc);

    register #(32) Pipeline_rd2_data(P_rd2_data, Forward_rd2_data, clk, 1'b1, stall|reset|PCSrc);
    alu32 alu(alu_out_data, zero, ALUOp, Forward_rd1_data, B_data);

    register #(32) Pipeline_alu_out_data(P_alu_out_data, alu_out_data, clk, 1'b1, stall|reset|PCSrc);

    // DO NOT comment out or rename this module
    // or the test bench will break
    data_mem data_memory(load_data, P_alu_out_data, P_rd2_data, P_MemRead, P_MemWrite, clk, reset);

    mux2v #(32) wb_mux(wr_data, P_alu_out_data, load_data, P_MemToReg);
    mux2v #(5) rd_mux(wr_regnum, rt, rd, RegDst);
    register #(5) Pipeline_wr_regnum(P_wr_regnum, wr_regnum, clk, 1'b1, stall|reset|PCSrc);

endmodule // pipelined_machine
