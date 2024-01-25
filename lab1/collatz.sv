module collatz(
    input logic         clk,   // Clock
    input logic         go,    // Load value from n; start iterating
    input logic [31:0]  n,     // Start value; only read when go = 1
    output logic [31:0] dout,  // Iteration value: true after go = 1
    output logic        done   // True when dout reaches 1
);

    logic [31:0] current;

    logic computation_done;

    // Assign the output signals to the internal state variables.
    assign dout = current;
    assign done = computation_done;

    // Sequential logic to perform the Collatz conjecture algorithm.
    always_ff @(posedge clk) begin
        if (go) begin
            // Reset the state and start the computation.
            current <= n;
            computation_done <= 1'b0;
        end else if (current != 32'd1 && !computation_done) begin
            // Perform the Collatz conjecture computation.
            if (current[0] == 1'b0) begin // Check if even
                current <= current >> 1; // Divide by 2
            end else begin
                current <= (current * 3) + 1; // Multiply by 3 and add 1
            end
        end

        // Check if the current value has reached 1.
        if (current == 32'd1) begin
            computation_done <= 1'b1; // Assert 'done' when computation is complete.
        end
    end

endmodule
