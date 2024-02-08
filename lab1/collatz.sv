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
		if(n==0)
			dout <= 32'd1;
		else
            	dout <= n;
        end
	else if (!done) begin
            // core logic of Collatz conjecture.
            if (dout[0] == 1'b0) begin
                dout <= dout >> 1;
            end else begin
                dout <= (dout * 3) + 1;
            end
        end
    end

assign done = (!go && (dout == 32'd1));
endmodule
