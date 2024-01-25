// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrange.h for the primary calling header

#include "verilated.h"

#include "Vrange__Syms.h"
#include "Vrange___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrange___024root___dump_triggers__act(Vrange___024root* vlSelf);
#endif  // VL_DEBUG

void Vrange___024root___eval_triggers__act(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.at(0U) = ((IData)(vlSelf->clk) 
                                      & (~ (IData)(vlSelf->__Vtrigrprev__TOP__clk)));
    vlSelf->__Vtrigrprev__TOP__clk = vlSelf->clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vrange___024root___dump_triggers__act(vlSelf);
    }
#endif
}
