// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcollatz.h for the primary calling header

#include "verilated.h"

#include "Vcollatz___024root.h"

VL_ATTR_COLD void Vcollatz___024root___eval_static(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vcollatz___024root___eval_initial(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigrprev__TOP__clk = vlSelf->clk;
}

VL_ATTR_COLD void Vcollatz___024root___eval_final(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_final\n"); );
}

VL_ATTR_COLD void Vcollatz___024root___eval_triggers__stl(Vcollatz___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__stl(Vcollatz___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___eval_stl(Vcollatz___024root* vlSelf);

VL_ATTR_COLD void Vcollatz___024root___eval_settle(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_settle\n"); );
    // Init
    CData/*0:0*/ __VstlContinue;
    // Body
    vlSelf->__VstlIterCount = 0U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        __VstlContinue = 0U;
        Vcollatz___024root___eval_triggers__stl(vlSelf);
        if (vlSelf->__VstlTriggered.any()) {
            __VstlContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VstlIterCount))) {
#ifdef VL_DEBUG
                Vcollatz___024root___dump_triggers__stl(vlSelf);
#endif
                VL_FATAL_MT("collatz.sv", 1, "", "Settle region did not converge.");
            }
            vlSelf->__VstlIterCount = ((IData)(1U) 
                                       + vlSelf->__VstlIterCount);
            Vcollatz___024root___eval_stl(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__stl(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VstlTriggered.at(0U)) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcollatz___024root___stl_sequent__TOP__0(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___stl_sequent__TOP__0\n"); );
    // Body
    vlSelf->dout = vlSelf->collatz__DOT__current;
    vlSelf->done = vlSelf->collatz__DOT__computation_done;
}

VL_ATTR_COLD void Vcollatz___024root___eval_stl(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_stl\n"); );
    // Body
    if (vlSelf->__VstlTriggered.at(0U)) {
        Vcollatz___024root___stl_sequent__TOP__0(vlSelf);
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__act(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VactTriggered.at(0U)) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__nba(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VnbaTriggered.at(0U)) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcollatz___024root___ctor_var_reset(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->go = VL_RAND_RESET_I(1);
    vlSelf->n = VL_RAND_RESET_I(32);
    vlSelf->dout = VL_RAND_RESET_I(32);
    vlSelf->done = VL_RAND_RESET_I(1);
    vlSelf->collatz__DOT__current = VL_RAND_RESET_I(32);
    vlSelf->collatz__DOT__computation_done = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigrprev__TOP__clk = VL_RAND_RESET_I(1);
}
