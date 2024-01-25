#include <iostream>
#include "Vrange.h"
#include <verilated.h>
#include <verilated_vcd_c.h>

int main(int argc, const char ** argv, const char ** env) {
  Verilated::commandArgs(argc, argv);

  int n;
  if (argc > 1 && argv[1][0] != '+')
    n = atoi(argv[1]);
  else
    n = 7;

  Vrange * dut = new Vrange;
  
  Verilated::traceEverOn(true);
  VerilatedVcdC * tfp = new VerilatedVcdC;
  dut->trace(tfp, 99);
  tfp->open("range.vcd");

  dut->clk = 0;
  dut->go = 0;
  dut->start = n;

  int time = 0;
  for ( ; time < 100000 ; time += 10) {
    dut->clk = ((time % 20) >= 10) ? 1 : 0;
    if (time == 0) dut->go = 1;
    if (time == 40) dut->go = 0;

    dut->eval();
    
    tfp->dump( time );

    if (dut->done) break;
  }

  for (int i = 0 ; i < 16 ; i++) {
    dut->clk = 0;
    dut->start = i;
    time += 10;
    
    dut->eval();
    tfp->dump( time );

    dut->clk = 1;
    time += 10;
    
    dut->eval();
    tfp->dump( time );

    std::cout << i + n << ' ' << dut->count << std::endl;
  }    
  
  tfp->close();
  delete tfp;

  dut->final();
  delete dut;

  return 0;
}

