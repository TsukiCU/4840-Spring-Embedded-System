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
   assign done = cdone;
   assign cgo = go;
   assign n = start;
   assign din = 16'h0;
   assign num = 0;
   assign we = running;   
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
	     
