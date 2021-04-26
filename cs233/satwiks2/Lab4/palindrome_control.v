
module palindrome_control(palindrome, done, select, load, go, a_ne_b, front_ge_back, clock, reset);
	output load, select, palindrome, done;
	input go, a_ne_b, front_ge_back;
	input clock, reset;

    wire sGarbage, sPal, sNotPal, sComp, sLoad;

    wire sGarbage_next = reset | sGarbage & ~go;
    wire sPal_next = (sComp & front_ge_back);
    wire sNot_pal_next = (sComp & a_ne_b & ~front_ge_back);
    wire sComp_next = (sLoad & ~go | sComp & ~a_ne_b & ~front_ge_back);
    wire sLoad_next = (sGarbage & go | sLoad & go);

    dffe fsGarbage(sGarbage, sGarbage_next, clock, 1'b1, 1'b0);
    dffe fsLoad(sLoad, sLoad_next, clock, 1'b1, 1'b0);
    dffe fsComp(sComp, sComp_next, clock, 1'b1, 1'b0);
    dffe fsPal(sPal, sPal_next, clock, 1'b1, 1'b0);
    dffe fsNotPal(sNotPal, sNot_pal_next, clock, 1'b1, 1'b0);

    assign palindrome = sPal;
    assign done = (sPal|sNotPal);
    assign select = sComp;
    assign load = (sComp|sLoad);


endmodule // palindrome_control 
