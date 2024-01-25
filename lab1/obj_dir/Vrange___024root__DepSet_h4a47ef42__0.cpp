// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrange.h for the primary calling header

#include "verilated.h"

#include "Vrange___024root.h"

void Vrange___024root___eval_act(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vrange___024root___nba_sequent__TOP__0(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___nba_sequent__TOP__0\n"); );
    // Init
    IData/*31:0*/ __Vdly__range__DOT__c1__DOT__current_value;
    __Vdly__range__DOT__c1__DOT__current_value = 0;
    CData/*0:0*/ __Vdly__range__DOT__c1__DOT__computation_done;
    __Vdly__range__DOT__c1__DOT__computation_done = 0;
    // Body
    __Vdly__range__DOT__c1__DOT__current_value = vlSelf->range__DOT__c1__DOT__current_value;
    __Vdly__range__DOT__c1__DOT__computation_done = vlSelf->range__DOT__c1__DOT__computation_done;
    if (vlSelf->go) {
        __Vdly__range__DOT__c1__DOT__current_value 
            = vlSelf->start;
        __Vdly__range__DOT__c1__DOT__computation_done = 0U;
    } else if (((1U != vlSelf->range__DOT__c1__DOT__current_value) 
                & (~ (IData)(vlSelf->range__DOT__c1__DOT__computation_done)))) {
        __Vdly__range__DOT__c1__DOT__current_value 
            = ((1U & vlSelf->range__DOT__c1__DOT__current_value)
                ? ((IData)(1U) + ((IData)(3U) * vlSelf->range__DOT__c1__DOT__current_value))
                : (vlSelf->range__DOT__c1__DOT__current_value 
                   >> 1U));
    }
    if ((1U == vlSelf->range__DOT__c1__DOT__current_value)) {
        __Vdly__range__DOT__c1__DOT__computation_done = 1U;
    }
    vlSelf->count = vlSelf->range__DOT__mem[(0xfU & vlSelf->start)];
    vlSelf->range__DOT__c1__DOT__current_value = __Vdly__range__DOT__c1__DOT__current_value;
    vlSelf->range__DOT__c1__DOT__computation_done = __Vdly__range__DOT__c1__DOT__computation_done;
    vlSelf->done = vlSelf->range__DOT__c1__DOT__computation_done;
}

void Vrange___024root___eval_nba(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vrange___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vrange___024root___eval_triggers__act(Vrange___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vrange___024root___dump_triggers__act(Vrange___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vrange___024root___dump_triggers__nba(Vrange___024root* vlSelf);
#endif  // VL_DEBUG

void Vrange___024root___eval(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___eval\n"); );
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
            Vrange___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vrange___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("range.sv", 1, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vrange___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vrange___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("range.sv", 1, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vrange___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vrange___024root___eval_debug_assertions(Vrange___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->go & 0xfeU))) {
        Verilated::overWidthError("go");}
}
#endif  // VL_DEBUG
