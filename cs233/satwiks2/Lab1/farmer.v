// Complete the farmer module in this file
// Don't put any code in this file besides the farmer circuit

module farmer(e, f, x, g, b);
    output e;
    input f, x, g, b;
    wire w1, w2, w3, w4, goat_eaten, bean_eaten;

    xor (w1, f, x); // If farmer not with fox
    xnor (w2, x, g); // and fox with goat then eaten
    and a1 (goat_eaten, w1, w2); //if both w1 and w2 are true then goat eaten

    xor (w3, f, g); // If farmer not with goat
    xnor (w4, g, b); // and goat with beans
    and a2 (bean_eaten, w3, w4); // if both w3 and w4 are true then bean gets eaten

    or o1(e, goat_eaten, bean_eaten); //if anyone got eaten answer is one
endmodule // farmer
