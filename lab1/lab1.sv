// CSEE 4840 Lab 1: Run and Display Collatz Conjecture Iteration Counts
//
// Spring 2024
//
// By: Yunzhou Li, Hongyu Sun
// Uni: yl5407, hs3475

module lab1( input logic        CLOCK_50,  // 50 MHz Clock input
	     
	     input logic [3:0] 	KEY, // Pushbuttons; KEY[0] is rightmost

	     input logic [9:0] 	SW, // Switches; SW[0] is rightmost

	     // 7-segment LED displays; HEX0 is rightmost
	     output logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5,

	     output logic [9:0] LEDR // LEDs above the switches; LED[0] on right
	     );

   logic 			clk, go, done;   
   logic [31:0] 		start;
   logic [15:0] 		count;

   logic [11:0] 		n;
   
   assign clk = CLOCK_50;
 
   range #(256, 8) // RAM_WORDS = 256, RAM_ADDR_BITS = 8)
         r ( .* ); // Connect everything with matching names

   // Replace this comment and the code below it with your own code;
   // The code below is merely to suppress Verilator lint warnings

	// Meaningless code used to avoid warning
	assign start[31:28] = count[15:12];

	logic [7:0]				addr;		// RAM address
	logic [9:0]				base;		// Base number
	logic [11:0]			o_count;	// Screen output count
	logic [24:0]			counter;	// Button timer
	logic					first;		// Button first

	// Screen
	// Bind count to right 3 digits
	// Bind num to left 3 digits
	hex7seg h0(.a(o_count[3:0]),.y(HEX0)),
		h1(.a(o_count[7:4]),.y(HEX1)),
		h2(.a(o_count[11:8]),.y(HEX2)),
		h3(.a(n[3:0]),.y(HEX3)),
		h4(.a(n[7:4]),.y(HEX4)),
		h5(.a(n[11:8]),.y(HEX5));

	// Wires
	// Bind LED lights to the switches
	assign LEDR = SW;
	// Output count
	assign o_count = done ? count[11:0] : 12'b0;
	// Output number
	assign n = done ? {2'b0,base[9:0]}+{4'b0,addr} : 12'b0;
	// Input value
	// If go, input number, else input address										
	assign start[27:0] = go ? {18'b0,SW} : {20'b0,addr};

	// Always at Posedge
	always_ff @(posedge CLOCK_50) begin
		// Start button
		if(!KEY[3]) begin
			go <= 1;					// Set start signal
			addr <= 8'b0;				// Init RAM address
			base <= SW[9:0];			// Set base number
		end
		// When button released, go becomes 0 to start computing
		else
			go <= 0;

		// Next button held
		if((!KEY[0]) ^ (!KEY[1])) begin
			counter <= counter + 25'b1;
			
			if (counter == 25'b0) begin // Debounce delay
				// Set first to 0
				first <= first && !first;
				// if first time, wait for 1<<25 cycles, about 800 ms
				// else, wait for 1<<23 cycles, about 200 ms
				counter <= first? 25'b1 : 25'h1800000;
				if(!KEY[0])
					addr <= addr + 8'b1;
				else if(!KEY[1])
					addr <= addr - 8'b1;
			end
		end 
		else begin
			// Set for fast press, about 50 ms
			counter <= 25'h1ffffff - 25'b1<<21;
			first <= 1'b1;
		end

		// Reset
		if(!KEY[2]) begin
			addr <= 0;
		end
	end
   
  
endmodule
