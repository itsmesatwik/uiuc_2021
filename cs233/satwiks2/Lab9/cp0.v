`define STATUS_REGISTER 5'd12
`define CAUSE_REGISTER  5'd13
`define EPC_REGISTER    5'd14

module cp0(rd_data, EPC, TakenInterrupt,
           wr_data, regnum, next_pc,
           MTC0, ERET, TimerInterrupt, clock, reset);
    output [31:0] rd_data;
    output [29:0] EPC;
    output        TakenInterrupt;
    input  [31:0] wr_data;
    input   [4:0] regnum;
    input  [29:0] next_pc;
    input         MTC0, ERET, TimerInterrupt, clock, reset;
    wire   [31:0] user_status, regnum_decode;

    wire [31:0] status_register = {{{{16'b0, user_status[15:8]}, 6'b0}, exception_level},user_status[0]};
    wire [31:0] cause_register = {{16'b0, TimerInterrupt}, 15'b0};
    wire exception_level;
    wire user_enable = regnum_decode[12];
    wire epc_enable = regnum_decode[14] || TakenInterrupt;
    wire   [29:0] EPC_d;
    assign TakenInterrupt = (user_status[15] & TimerInterrupt) & (user_status[0] & ~exception_level);
    wire excep_enable = reset | ERET;
    decoder32 Regnum_decode(regnum_decode, regnum, MTC0);
    register #(32) User_status (user_status, wr_data, clock, user_enable, reset);
    register #(1) Exception_level (exception_level,1'b1, clock,TakenInterrupt, excep_enable);
    register #(30) EPC_register (EPC, EPC_d, clock, epc_enable, reset);
    wire [31:0]  mux_in = {EPC, 2'b0};
    mux2v #(30) mux1(EPC_d, wr_data[31:2], next_pc, TakenInterrupt);
    mux32v #(32) Rd_data (rd_data,0,0,0,0,0,0,0,0,0,0,0,0,status_register,cause_register,mux_in,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, regnum);





    // your Verilog for coprocessor 0 goes here
endmodule
