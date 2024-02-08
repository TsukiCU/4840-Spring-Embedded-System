module collatz( input logic         clk,   // Clock
		input logic 	    go,    // Load value from n; start iterating
		input logic  [31:0] n,     // Start value; only read when go = 1
		output logic [31:0] dout,  // Iteration value: true after go = 1
		output logic 	    done); // True when dout reaches 1

   /* Replace this comment and the code below with your solution */
	assign done = (!go && (dout == 32'd1));

	/* triggered by rising edge */
	always_ff @(posedge clk) begin
		// Reset the state and start the computation.
		if (go) begin
			if(n==0)
				dout <= 32'd1;
			else
				dout <= n;
		end
		// core logic of Collatz conjecture.
		else if (!done) begin
			if (dout[0] == 1'b0) begin
				dout <= dout >> 1;
			end else begin
				dout <= (dout * 3) + 1;
			end
		end
	end
   /* Replace this comment and the code above with your solution */

endmodule
