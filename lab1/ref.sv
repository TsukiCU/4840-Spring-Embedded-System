// verilator lint_off UNUSED

module range
   #(parameter
     RAM_WORDS = 16,            // Number of counts to store in RAM
     RAM_ADDR_BITS = 4)         // Number of RAM address bits
   (input logic         clk,    // Clock
    input logic         go,     // Read start and start testing
    input logic [31:0]  start,  // Number to start from or for reading the count
    output logic        done,   // True once memory is filled
    output logic [15:0] count); // Iteration count once finished

   logic               cgo, cdone;
   logic [31:0]        n;
   logic [RAM_ADDR_BITS-1:0] num;
   logic               running;
   logic [15:0]        din;
   logic               we;
   logic [31:0]        dummy_dout; // Dummy variable for dout
   logic [15:0]        mem[RAM_WORDS-1:0]; // The RAM

   // Instantiate the Collatz iterator with dummy_dout for dout
   collatz c1(.clk(clk),
              .go(cgo),
              .n(n),
              .dout(dummy_dout),
              .done(cdone));

   // Logic for managing the iteration process and storing the results in mem
   always_ff @(posedge clk) begin
      if (go && !running) begin
         running <= 1;
         n <= start;
         num <= 0;
         din <= 0; // Start iteration count at 0
         cgo <= 1; // Pulse cgo to start collatz
         done <= 0; // Ensure done is low initially
      end else if (running) begin
         cgo <= 0; // Ensure cgo is pulsed only once per iteration
         if (cdone) begin
            we <= 1; // Enable write for a single cycle
            if (we) begin
               mem[num] <= din; // Store the iteration count
               if (num == RAM_WORDS[RAM_ADDR_BITS-1:0]-1) begin
                  running <= 0;
                  done <= 1; // Pulse done signal to indicate completion
                  we <= 0; // Disable write after storing the last value
               end else begin
                  num <= num + 1;
                  n <= n + 1; // Move to the next number
                  din <= 0; // Reset iteration count for the next number
                  cgo <= 1; // Start next Collatz iteration
               end
            end
         end else if (we) begin
            we <= 0; // Disable write after one cycle
         end else if (!cgo && !cdone) begin
            din <= din + 1; // Increment din until cdone is asserted
         end
      end
   end

   // Logic for reading from mem when done is asserted and go is not
   always_ff @(posedge clk) begin
      if (done) begin
         // Use start as the address to read from mem when done is asserted
         count <= mem[start[RAM_ADDR_BITS-1:0]]; // Correctly read from RAM
      end
   end
endmodule
