// mips_decode: a decoder for MIPS arithmetic instructions
//
// alu_op      (output) - control signal to be sent to the ALU
// writeenable (output) - should a new value be captured by the register file
// rd_src      (output) - should the destination register be rd (0) or rt (1)
// alu_src2    (output) - should the 2nd ALU source be a register (0) or an immediate (1)
// except      (output) - set to 1 when the opcode/funct combination is unrecognized
// opcode      (input)  - the opcode field from the instruction
// funct       (input)  - the function field from the instruction

module mips_decode(alu_op, writeenable, rd_src, alu_src2, except, opcode, funct);
    output [2:0] alu_op;
    output       writeenable, rd_src, alu_src2, except;
    input  [5:0] opcode, funct;
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
    assign alu_src2 = (addi_|andi_|ori_|xori_);
    assign except = (~add_&~sub_&~and_&~or_&~nor_&~xor_&~addi_&~andi_&~ori_&~xori_);
    assign rd_src = (addi_|andi_|ori_|xori_);
    assign writeenable = ~except;
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
            0;
    // Use truth table for each individual bit of alu_op
    // Check if atleast one op was flagged as 1
    // rd_src is just if it was i type or r type


endmodule // mips_decode
