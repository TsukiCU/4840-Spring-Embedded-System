// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcollatz.h for the primary calling header

#include "verilated.h"

#include "Vcollatz___024root.h"

void Vcollatz___024root___eval_act(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vcollatz___024root___nba_sequent__TOP__0(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___nba_sequent__TOP__0\n"); );
    // Init
    IData/*31:0*/ __Vdly__dout;
    __Vdly__dout = 0;
    CData/*0:0*/ __Vdly__done;
    __Vdly__done = 0;
    // Body
    __Vdly__done = vlSelf->done;
    __Vdly__dout = vlSelf->dout;
    if (vlSelf->go) {
        __Vdly__dout = vlSelf->n;
        __Vdly__done = 0U;
    } else if (((1U != vlSelf->dout) & (~ (IData)(vlSelf->done)))) {
        __Vdly__dout = ((1U & vlSelf->dout) ? ((IData)(1U) 
                                               + ((IData)(3U) 
                                                  * vlSelf->dout))
                         : (vlSelf->dout >> 1U));
    }
    if ((1U == vlSelf->dout)) {
        __Vdly__done = 1U;
    }
    vlSelf->dout = __Vdly__dout;
    vlSelf->done = __Vdly__done;
}

void Vcollatz___024root___eval_nba(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vcollatz___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vcollatz___024root___eval_triggers__act(Vcollatz___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__act(Vcollatz___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcollatz___024root___dump_triggers__nba(Vcollatz___024root* vlSelf);
#endif  // VL_DEBUG

void Vcollatz___024root___eval(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vcollatz___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vcollatz___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("collatz.sv", 1, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vcollatz___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vcollatz___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("collatz.sv", 1, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vcollatz___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vcollatz___024root___eval_debug_assertions(Vcollatz___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcollatz___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->go & 0xfeU))) {
        Verilated::overWidthError("go");}
}
#endif  // VL_DEBUG
