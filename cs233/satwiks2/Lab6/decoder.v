

// mips_decode: a decoder for MIPS arithmetic instructions
//
// alu_op       (output) - control signal to be sent to the ALU
// writeenable  (output) - should a new value be captured by the register file
// rd_src       (output) - should the destination register be rd (0) or rt (1)
// alu_src2     (output) - should the 2nd ALU source be a register (0) or an immediate (1)
// except       (output) - set to 1 when we don't recognize an opdcode & funct combination
// control_type (output) - 00 = fallthrough, 01 = branch_target, 10 = jump_target, 11 = jump_register
// mem_read     (output) - the register value written is coming from the memory
// word_we      (output) - we're writing a word's worth of data
// byte_we      (output) - we're only writing a byte's worth of data
// byte_load    (output) - we're doing a byte load
// lui          (output) - the instruction is a lui
// slt          (output) - the instruction is an slt
// addm         (output) - the instruction is an addm
// opcode        (input) - the opcode field from the instruction
// funct         (input) - the function field from the instruction
// zero          (input) - from the ALU
//

module mips_decode(alu_op, writeenable, rd_src, alu_src2, except, control_type,
                   mem_read, word_we, byte_we, byte_load, lui, slt, addm,
                   opcode, funct, zero);
    output [2:0] alu_op;
    output       writeenable, rd_src, alu_src2, except;
    output [1:0] control_type;
    output       mem_read, word_we, byte_we, byte_load, lui, slt, addm;
    input  [5:0] opcode, funct;
    input        zero;


    // output [2:0] alu_op;
    // output       writeenable, rd_src, alu_src2, except;
    // input  [5:0] opcode, funct;
    wire add_ = (opcode == 6'b0 & funct == `OP0_ADD);
    wire sub_ = (opcode == 6'b0 & funct == `OP0_SUB);
    wire and_ = (opcode == 6'b0 & funct == `OP0_AND);
    wire or_ = (opcode == 6'b0 & funct == `OP0_OR);
    wire nor_ = (opcode == 6'b0 & funct == `OP0_NOR);
    wire xor_ = (opcode == 6'b0 & funct == `OP0_XOR);
    wire addi_ = (opcode == `OP_ADDI);
    wire andi_ = (opcode == `OP_ANDI);
    wire ori_ = (opcode == `OP_ORI);
    wire xori_ = (opcode == `OP_XORI);



    wire j_ = (opcode == `OP_J);
    wire beq_ = (opcode == `OP_BEQ);
    wire bne_ = (opcode == `OP_BNE);
    wire jr_ = (opcode == `OP_OTHER0 & funct == `OP0_JR);
    wire lui_ = (opcode == `OP_LUI);
    wire slt_ = (opcode == `OP_OTHER0 & funct == `OP0_SLT);
    wire lw_ = (opcode == `OP_LW);
    wire lbu_ = (opcode == `OP_LBU);
    wire sw_ = (opcode == `OP_SW);
    wire sb_ = (opcode == `OP_SB);
    wire addm_ = (opcode == `OP_OTHER0 & funct == `OP0_ADDM);


    assign word_we = sw_;
    assign byte_we = sb_;
    assign byte_load = lbu_;
    assign mem_read = lw_ | lbu_ | addm_;
    assign lui = lui_;
    assign slt = slt_;
    assign addm = addm_;

    assign alu_src2 = (addi_|andi_|ori_|xori_|lw_|lbu_|sw_|sb_);

    assign except = (~add_&~sub_&~and_&~or_&~nor_&~xor_&~addi_&~andi_&~ori_&~xori_&
                    ~ j_&~beq_&~bne_&~jr_&~lui_&~slt_&~lw_&~lbu_&~sw_&~sb_&~addm_);

    assign rd_src = (addi_|andi_|ori_|xori_|lw_|lui_|lbu_);

    assign writeenable = (add_|sub_|and_|or_|nor_|xor_|addi_|andi_|ori_|xori_|slt_|lbu_|lw_|lui_|addm_);
    assign control_type = (bne_&~zero) ? 2'b01 :
            (beq_&zero) ? 2'b01 :
            j_   ? 2'b10 :
            jr_  ? 2'b11 :
            0;

    assign alu_op = add_ ? 3'b010 :
            sub_ ? 3'b011 :
            and_ ? 3'b100 :
            or_  ? 3'b101 :
            nor_ ? 3'b110 :
            xor_ ? 3'b111 :
            addi_? 3'b010 :
            andi_? 3'b100 :
            ori_ ? 3'b101 :
            xori_? 3'b111 :
            beq_ ? 3'b011 :
            bne_ ? 3'b011 :
            slt_ ? 3'b011 :
            lw_  ? 3'b010 :
            lbu_ ? 3'b010 :
            sw_  ? 3'b010 :
            sb_  ? 3'b010 :
            addm_? 3'b010 :
            0;

endmodule // mips_decode
