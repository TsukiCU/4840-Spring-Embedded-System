#include <iostream>
#include <iomanip>
#include "Vhex7seg.h"
#include <verilated.h>

int main(int argc, const char ** argv, const char ** env) {
    Verilated::commandArgs(argc, argv);

    Vhex7seg* dut = new Vhex7seg;

    dut->a = 0x89;

    dut->eval();

    unsigned char result = dut->y;
    std::cout << "Input: 0x89, Output: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)result << std::endl;

    if (result == 0x89) {
        std::cout << "Test passed." << std::endl;
    } else {
        std::cout << "Test failed. Expected 0x89, got 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)result << std::endl;
    }

    dut->final();
    delete dut;

    return 0;
}
