// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vcollatz__Syms.h"


VL_ATTR_COLD void Vcollatz___024root__trace_init_sub__TOP__0(Vcollatz___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clk", false,-1);
    tracep->declBit(c+2,"go", false,-1);
    tracep->declBus(c+3,"n", false,-1, 31,0);
    tracep->declBus(c+4,"dout", false,-1, 31,0);
    tracep->declBit(c+5,"done", false,-1);
    tracep->pushNamePrefix("collatz ");
    tracep->declBit(c+1,"clk", false,-1);
    tracep->declBit(c+2,"go", false,-1);
    tracep->declBus(c+3,"n", false,-1, 31,0);
    tracep->declBus(c+4,"dout", false,-1, 31,0);
    tracep->declBit(c+5,"done", false,-1);
    tracep->declBus(c+6,"current", false,-1, 31,0);
    tracep->declBit(c+7,"computation_done", false,-1);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void Vcollatz___024root__trace_init_top(Vcollatz___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root__trace_init_top\n"); );
    // Body
    Vcollatz___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vcollatz___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcollatz___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcollatz___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vcollatz___024root__trace_register(Vcollatz___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vcollatz___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vcollatz___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vcollatz___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vcollatz___024root__trace_full_sub_0(Vcollatz___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vcollatz___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root__trace_full_top_0\n"); );
    // Init
    Vcollatz___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcollatz___024root*>(voidSelf);
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vcollatz___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vcollatz___024root__trace_full_sub_0(Vcollatz___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clk));
    bufp->fullBit(oldp+2,(vlSelf->go));
    bufp->fullIData(oldp+3,(vlSelf->n),32);
    bufp->fullIData(oldp+4,(vlSelf->dout),32);
    bufp->fullBit(oldp+5,(vlSelf->done));
    bufp->fullIData(oldp+6,(vlSelf->collatz__DOT__current),32);
    bufp->fullBit(oldp+7,(vlSelf->collatz__DOT__computation_done));
}
