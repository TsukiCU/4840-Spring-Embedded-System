module collatz(
    input logic         clk,   // Clock
    input logic         go,    // Load value from n; start iterating
    input logic [31:0]  n,     // Start value; only read when go = 1
    output logic [31:0] dout,  // Iteration value: true after go = 1
    output logic        done   // True when dout reaches 1
);

    /* triggered by rising edge */
    always_ff @(posedge clk) begin
        if (go) begin
            // Reset the state and start the computation.
            dout <= n;
            done <= 1'b0;
        end else if (dout != 32'd1 && !done) begin
            // core logic of Collatz conjecture.
            if (dout[0] == 1'b0) begin
                dout <= dout >> 1;
            end else begin
                dout <= (dout * 3) + 1;
            end
        end

        if (dout == 32'd2) begin
            done <= 1'b1;
        end
    end

endmodule
