module hex7seg(input logic [3:0] a,
               output logic [6:0] y);

   /* Replace this comment and the code below it with your solution */
   always_comb
    begin
        case (a)
            4'h0: y[6:0] = 7'b1000000;
            4'h1: y[6:0] = 7'b1111001;
            4'h2: y[6:0] = 7'b0100100;
            4'h3: y[6:0] = 7'b0110000;
            4'h4: y[6:0] = 7'b0011001;
            4'h5: y[6:0] = 7'b0010010;
            4'h6: y[6:0] = 7'b0000010;
            4'h7: y[6:0] = 7'b1111000;
            4'h8: y[6:0] = 7'b0000000;
            4'h9: y[6:0] = 7'b0010000;
            4'hA: y[6:0] = 7'b0001000;
            4'hB: y[6:0] = 7'b0000011;
            4'hC: y[6:0] = 7'b1000110;
            4'hD: y[6:0] = 7'b0100001;
            4'hE: y[6:0] = 7'b0000110;
            default : y[6:0] = 7'b0001110;
        endcase
    end
endmodule
