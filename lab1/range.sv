module range
   #(parameter
     RAM_WORDS = 16,            // Number of counts to store in RAM
     RAM_ADDR_BITS = 4)         // Number of RAM address bits
   (input logic         clk,    // Clock
    input logic 	go,     // Read start and start testing
    input logic [31:0] 	start,  // Number to start from or count to read
    output logic 	done,   // True once memory is filled
    output logic [15:0] count); // Iteration count once finished

   logic 		cgo;    // "go" for the Collatz iterator
   logic                cdone;  // "done" from the Collatz iterator
   logic [31:0] 	n;      // number to start the Collatz iterator
   logic 		cdone_prev; // Previous value of cdone

// verilator lint_off PINCONNECTEMPTY

   // Instantiate the Collatz iterator
   collatz c1(.clk(clk),
	      .go(cgo),
	      .n(n),
	      .done(cdone),
	      .dout());

   logic [RAM_ADDR_BITS - 1:0] 	 num;         // The RAM address to write
   logic 			 running = 0; // True during the iterations

   /* Replace this comment and the code below with your solution,
      which should generate running, done, cgo, n, num, we, and din */
   assign we = running;
   always_ff @(posedge clk) begin
	// Reset
	if (go) begin
		n<=start;				// Init first number
		num<=0;					// Init RAM address
		running<=1;				// Iteration Running
		din<=16'h0;				// Init count
		cgo <= 1;				// Start collatz
		done <= 0;				// Init done
		cdone_prev <= 0;			// Initialize cdone_prev
	end
	// One number Complete
	else if (cdone && !cdone_prev) begin
		// If RAM full, give done signal
		if (num == ((1 << RAM_ADDR_BITS) -1)) begin
			done <=1;
			running<=0;			// Iteration Done
		end
		// Init next iteration
		else begin
			num<=num+1;			// Move RAM Address
			n<=n+1;				// Next number
			din<=16'h0;			// Init count
			cgo <= 1;			// Start collatz
			//cdone <= 0;			// Init cdone
		end
	end
	// Computing
	else if (!done) begin
		cgo <= 0;				// cgo should be 0
		din <= din+1;				// Count++
	end
	cdone_prev <= cdone;
   end
   /* Replace this comment and the code above with your solution */

   logic 			 we;                    // Write din to addr
   logic [15:0] 		 din;                   // Data to write
   logic [15:0] 		 mem[RAM_WORDS - 1:0];  // The RAM itself
   logic [RAM_ADDR_BITS - 1:0] 	 addr;                  // Address to read/write

   assign addr = we ? num : start[RAM_ADDR_BITS-1:0];

   always_ff @(posedge clk) begin
      if (we) mem[addr] <= din;
      count <= mem[addr];
   end

endmodule