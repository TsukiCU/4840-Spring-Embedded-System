
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
   // verilator lint_off UNUSED
   logic [31:0]        dummy_dout; // Dummy variable for dout. Put this here to make the stupid verilator happy.
   // verilator lint_on UNUSED
   logic [15:0]        mem[RAM_WORDS-1:0];
   logic               initiate_new_cycle; // Signal to initiate a new Collatz cycle.

   collatz c1(.clk(clk),
              .go(cgo),
              .n(n),
              .dout(dummy_dout),
              .done(cdone));

   always_ff @(posedge clk) begin
      if (go && !running) begin
         running <= 1;
         n <= start;
         num <= 0;
         din <= 0; // Start iteration count at 0
         cgo <= 1; // Pulse cgo to start collatz
         done <= 0; // Ensure done is low initially
         initiate_new_cycle <= 0;
      end else if (running) begin
         if (cdone) begin
            cdone <= 0;
            we <= 1;
            /* TODO: optimize this if we have time. This is the opposite of elegance but somehow just works. */
            if (num == 0) mem[num] <= din;
            else mem[num] <= din+1;
            if ({28'b0, num} == RAM_WORDS-1) begin
               running <= 0;
               done <= 1;
            end else begin
               num <= num + 1;
               n <= n + 1;
               din <= 0;
               initiate_new_cycle <= 1; // Set flag to initiate a new cycle
            end
         end else if (initiate_new_cycle) begin
            cgo <= 1;
            initiate_new_cycle <= 0; // Reset flag after initiating
         end else begin
            cgo <= 0;
            if (we) begin
               we <= 0;
            end else begin
               din <= din + 1;
            end
         end
      end
   end

   always_ff @(posedge clk) begin
      if (done) begin
         count <= mem[start[RAM_ADDR_BITS-1:0]];
      end
   end
endmodule
