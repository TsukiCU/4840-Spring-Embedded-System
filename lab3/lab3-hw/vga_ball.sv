/*
 * Avalon memory-mapped peripheral that generates VGA
 *
 * Stephen A. Edwards
 * Columbia University
 */

module vga_ball(input logic        clk,
	        input logic 	   reset,
		input logic [7:0]  writedata,
		input logic 	   write,
		input 		   chipselect,
		input logic [3:0]  address,

		output logic [7:0] VGA_R, VGA_G, VGA_B,
		output logic 	   VGA_CLK, VGA_HS, VGA_VS,
		                   VGA_BLANK_n,
		output logic 	   VGA_SYNC_n);

   logic [10:0]	   hcount;
   logic [9:0]     vcount;

   logic [7:0] 	   background_r, background_g, background_b;
   logic [7:0]     circle_r, circle_g, circle_b;
   //logic [7:0]     rect_left,rect_top,rect_right,rect_bottom;
   logic [15:0]    circle_x,circle_y;
   logic [19:0]     circle_radius;
   logic [19:0]    dis2, r2;
   logic [19:0]     dis_x,dis_y;

   assign dis_x = (hcount[10:1] > circle_x[9:0]) ? (hcount[10:1] - circle_x[9:0]): (circle_x[9:0] - hcount[10:1]);
   assign dis_y = (vcount[9:0] > circle_y[9:0]) ? (vcount[9:0] - circle_y[9:0]): (circle_y[9:0] - vcount[9:0]);
   assign dis2 = $unsigned(dis_x)*$unsigned(dis_x) + 
                 $unsigned(dis_y)*$unsigned(dis_y);
   assign r2 = $unsigned(circle_radius)*$unsigned(circle_radius);
	
   vga_counters counters(.clk50(clk), .*);

   always_ff @(posedge clk)
     if (reset) begin
	background_r <= 8'h0;
	background_g <= 8'h0;
	background_b <= 8'h80;
  //rect_left <= 8'b11000000;
  //rect_top <= 8'b11000000;
  //rect_right <= 8'b11111111;
  //rect_bottom <= 8'b11111111;
    circle_x <= 16'h00000000;
    circle_y <= 16'h00000000;
    circle_radius <= 20'h0;
     end else if (chipselect && write)
       case (address)
        4'h0 : circle_r <= writedata;
        4'h1 : circle_g <= writedata;
        4'h2 : circle_b <= writedata;
        4'h3 : circle_x[15:8] <= writedata;
        4'h4 : circle_x[7:0] <= writedata;
        4'h5 : circle_y[15:8] <= writedata;
        4'h6 : circle_y[7:0] <= writedata;
        4'h7 : circle_radius[7:0] <= writedata;
        4'h8 : background_r <= writedata;
        4'h9 : background_g <= writedata;
        4'ha : background_b <= writedata;
       endcase

   always_comb begin
      {VGA_R, VGA_G, VGA_B} = {8'h0, 8'h0, 8'h0};
      if (VGA_BLANK_n )
	if (dis2<r2 && circle_x <= 16'd1280 && circle_y <= 16'd640)
	  {VGA_R, VGA_G, VGA_B} = {circle_r, circle_g, circle_b};
	else
	  {VGA_R, VGA_G, VGA_B} =
             {background_r,background_g,background_b};
   end
	       
endmodule

module vga_counters(
 input logic 	     clk50, reset,
 output logic [10:0] hcount,  // hcount[10:1] is pixel column
 output logic [9:0]  vcount,  // vcount[9:0] is pixel row
 output logic 	     VGA_CLK, VGA_HS, VGA_VS, VGA_BLANK_n, VGA_SYNC_n);

/*
 * 640 X 480 VGA timing for a 50 MHz clock: one pixel every other cycle
 * 
 * HCOUNT 1599 0             1279       1599 0
 *             _______________              ________
 * ___________|    Video      |____________|  Video
 * 
 * 
 * |SYNC| BP |<-- HACTIVE -->|FP|SYNC| BP |<-- HACTIVE
 *       _______________________      _____________
 * |____|       VGA_HS          |____|
 */
   // Parameters for hcount
   parameter HACTIVE      = 11'd 1280,
             HFRONT_PORCH = 11'd 32,
             HSYNC        = 11'd 192,
             HBACK_PORCH  = 11'd 96,   
             HTOTAL       = HACTIVE + HFRONT_PORCH + HSYNC +
                            HBACK_PORCH; // 1600
   
   // Parameters for vcount
   parameter VACTIVE      = 10'd 480,
             VFRONT_PORCH = 10'd 10,
             VSYNC        = 10'd 2,
             VBACK_PORCH  = 10'd 33,
             VTOTAL       = VACTIVE + VFRONT_PORCH + VSYNC +
                            VBACK_PORCH; // 525

   logic endOfLine;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          hcount <= 0;
     else if (endOfLine) hcount <= 0;
     else  	         hcount <= hcount + 11'd 1;

   assign endOfLine = hcount == HTOTAL - 1;
       
   logic endOfField;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          vcount <= 0;
     else if (endOfLine)
       if (endOfField)   vcount <= 0;
       else              vcount <= vcount + 10'd 1;

   assign endOfField = vcount == VTOTAL - 1;

   // Horizontal sync: from 0x520 to 0x5DF (0x57F)
   // 101 0010 0000 to 101 1101 1111
   assign VGA_HS = !( (hcount[10:8] == 3'b101) &
		      !(hcount[7:5] == 3'b111));
   assign VGA_VS = !( vcount[9:1] == (VACTIVE + VFRONT_PORCH) / 2);

   assign VGA_SYNC_n = 1'b0; // For putting sync on the green signal; unused
   
   // Horizontal active: 0 to 1279     Vertical active: 0 to 479
   // 101 0000 0000  1280	       01 1110 0000  480
   // 110 0011 1111  1599	       10 0000 1100  524
   assign VGA_BLANK_n = !( hcount[10] & (hcount[9] | hcount[8]) ) &
			!( vcount[9] | (vcount[8:5] == 4'b1111) );

   /* VGA_CLK is 25 MHz
    *             __    __    __
    * clk50    __|  |__|  |__|
    *        
    *             _____       __
    * hcount[0]__|     |_____|
    */
   assign VGA_CLK = hcount[0]; // 25 MHz clock: rising edge sensitive
   
endmodule
