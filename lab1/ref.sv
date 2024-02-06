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
   logic [15:0]        mem[RAM_WORDS-1:0]; // The RAM

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
      end else if (running) begin
         cgo <= 0; // Ensure cgo is pulsed only once per iteration
         if (cdone) begin
            we <= 1; // Enable write for a single cycle
            if (we) begin
               mem[num] <= din;
               if ({28'b0, num} == (RAM_WORDS-1)) begin
                  running <= 0;
                  done <= 1; // Pulse done signal to indicate completion
                  we <= 0; // Disable write after storing the last value
               end else begin
                  num <= num + 1;
                  n <= n + 1;
                  din <= 0;
                  cgo <= 1;
               end
            end
         end else if (we) begin
            we <= 0;
         end else if (!cgo && !cdone) begin
            din <= din + 1;
         end
      end
   end

   always_ff @(posedge clk) begin
      if (done) begin
         count <= mem[start[RAM_ADDR_BITS-1:0]];
      end
   end
endmodule
