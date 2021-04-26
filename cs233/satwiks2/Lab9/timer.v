module timer(TimerInterrupt, cycle, TimerAddress,
             data, address, MemRead, MemWrite, clock, reset);
    output        TimerInterrupt;
    output [31:0] cycle;
    output        TimerAddress;
    input  [31:0] data, address;
    input         MemRead, MemWrite, clock, reset;


    wire and_input1 = (address == 32'hffff006c);
    wire and_input2 = (address == 32'hffff001c);
    wire timer_write;
    wire timer_read, acknowledge;
    wire[31:0] Q1,Q2,D;
    wire enable2 = (Q1 == Q2);
    wire reset_;
    and timer_read_(timer_read, MemRead, and_input2);
    and timer_write_(timer_write, MemWrite, and_input2);
    and acknowledge_(acknowledge, and_input1, MemWrite);
    or timer_add_(TimerAddress, and_input1, and_input2);

    register #(32, 32'hffffffff) interrupt_cycle_(Q1, data, clock, timer_write, reset);
    alu32 alu_1_(D,,,3'b0,Q2,32'b1);
    register #(32) cycle_counter(Q2,D,clock,1'b1,reset);




    tristate t1_(cycle, Q2, timer_read);

    or o2(reset_, acknowledge, reset);
    register #(1) interrupt_line_(TimerInterrupt, 1'b1, clock, enable2, reset_);



    // complete the timer circuit here

    // HINT: make your interrupt cycle register reset to 32'hffffffff
    //       (using the reset_value parameter)
    //       to prevent an interrupt being raised the very first cycle
endmodule
