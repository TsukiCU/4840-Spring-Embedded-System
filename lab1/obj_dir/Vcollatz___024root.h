// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcollatz.h for the primary calling header

#ifndef VERILATED_VCOLLATZ___024ROOT_H_
#define VERILATED_VCOLLATZ___024ROOT_H_  // guard

#include "verilated.h"

class Vcollatz__Syms;

class Vcollatz___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(go,0,0);
    VL_OUT8(done,0,0);
    CData/*0:0*/ __Vtrigrprev__TOP__clk;
    CData/*0:0*/ __VactContinue;
    VL_IN(n,31,0);
    VL_OUT(dout,31,0);
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vcollatz__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcollatz___024root(Vcollatz__Syms* symsp, const char* v__name);
    ~Vcollatz___024root();
    VL_UNCOPYABLE(Vcollatz___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
