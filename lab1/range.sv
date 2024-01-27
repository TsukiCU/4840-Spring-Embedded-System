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
   //logic [31:0] collatz_dout;

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
   // assign done = cdone;
   // assign cgo = go;
   // assign n = start;
   // assign din = 16'h0;
   // assign we = running;
   /* Replace this comment and the code above with your solution */

   logic 			 we=0;                    // Write din to addr
   logic [15:0] 		 din=10;                   // Data to write
   logic [15:0] 		 mem[RAM_WORDS - 1:0];  // The RAM itself
   logic [RAM_ADDR_BITS - 1:0] 	 addr;                  // Address to read/write
   logic go_last;       // cgo capture the rising edge of go and cdone.
   logic cdone_last;    // we capture the rising edge of cdone.

   logic [15:0] mem_tmp;

   assign addr = we ? num : start[RAM_ADDR_BITS-1:0];

   always_ff @(posedge clk) begin
      if (we) begin
         mem[addr] <= din;
         we <= 0;
      end

      if (done)
         mem_tmp <= mem[addr];
      count <= mem_tmp;

      if (go) begin
         running <= 1;
         n <= start;
         num <= 0;
         din <= 1;
         if (!go_last) cgo <= 1;
         else cgo <= 0;
         go_last <= go;
      end

      if (running) begin  // running is constanly treu during iterations
         if (!cgo && !cdone) begin
         /* check if callatz is not active or its not done yet. */
              din <= din + 1;
          end

         if (cdone) begin
            /* cdone is set, we is pulsed high to write din in RAM. */
            if (!cdone_last) begin
               cgo <= 1;
               we <= 1;
            end
            else begin
               cgo <= 0;
               we <= 1;
            end
            cdone_last <= cdone;

            din <= 1;
            num <= num+1; // num serves as the address pointer?
            n <= n+1;
            cdone <= 0;
            // when to stop?
            if (num == RAM_WORDS[RAM_ADDR_BITS-1:0]-1) begin
               running <= 0;
               done <= 1;  // if num reaches limit, done is set, we are all good.
            end
         end

      end
   end

endmodule

