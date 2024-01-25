// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vrange__Syms.h"


void Vrange___024root__trace_chg_sub_0(Vrange___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vrange___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_chg_top_0\n"); );
    // Init
    Vrange___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vrange___024root*>(voidSelf);
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vrange___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void Vrange___024root__trace_chg_sub_0(Vrange___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->clk));
    bufp->chgBit(oldp+1,(vlSelf->go));
    bufp->chgIData(oldp+2,(vlSelf->start),32);
    bufp->chgBit(oldp+3,(vlSelf->done));
    bufp->chgSData(oldp+4,(vlSelf->count),16);
    bufp->chgCData(oldp+5,((0xfU & vlSelf->start)),4);
    bufp->chgIData(oldp+6,(vlSelf->range__DOT__c1__DOT__current_value),32);
    bufp->chgBit(oldp+7,(vlSelf->range__DOT__c1__DOT__computation_done));
}

void Vrange___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_cleanup\n"); );
    // Init
    Vrange___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vrange___024root*>(voidSelf);
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
