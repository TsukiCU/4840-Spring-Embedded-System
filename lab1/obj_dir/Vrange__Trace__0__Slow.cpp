// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vrange__Syms.h"


VL_ATTR_COLD void Vrange___024root__trace_init_sub__TOP__0(Vrange___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clk", false,-1);
    tracep->declBit(c+2,"go", false,-1);
    tracep->declBus(c+3,"start", false,-1, 31,0);
    tracep->declBit(c+4,"done", false,-1);
    tracep->declBus(c+5,"count", false,-1, 15,0);
    tracep->pushNamePrefix("range ");
    tracep->declBus(c+9,"RAM_WORDS", false,-1, 31,0);
    tracep->declBus(c+10,"RAM_ADDR_BITS", false,-1, 31,0);
    tracep->declBit(c+1,"clk", false,-1);
    tracep->declBit(c+2,"go", false,-1);
    tracep->declBus(c+3,"start", false,-1, 31,0);
    tracep->declBit(c+4,"done", false,-1);
    tracep->declBus(c+5,"count", false,-1, 15,0);
    tracep->declBit(c+2,"cgo", false,-1);
    tracep->declBit(c+4,"cdone", false,-1);
    tracep->declBus(c+3,"n", false,-1, 31,0);
    tracep->declBus(c+11,"num", false,-1, 3,0);
    tracep->declBit(c+12,"running", false,-1);
    tracep->declBit(c+12,"we", false,-1);
    tracep->declBus(c+13,"din", false,-1, 15,0);
    for (int i = 0; i < 16; ++i) {
        tracep->declBus(c+14+i*1,"mem", true,(i+0), 15,0);
    }
    tracep->declBus(c+6,"addr", false,-1, 3,0);
    tracep->pushNamePrefix("c1 ");
    tracep->declBit(c+1,"clk", false,-1);
    tracep->declBit(c+2,"go", false,-1);
    tracep->declBus(c+3,"n", false,-1, 31,0);
    tracep->declBus(c+7,"dout", false,-1, 31,0);
    tracep->declBit(c+4,"done", false,-1);
    tracep->declBus(c+7,"current_value", false,-1, 31,0);
    tracep->declBit(c+8,"computation_done", false,-1);
    tracep->popNamePrefix(2);
}

VL_ATTR_COLD void Vrange___024root__trace_init_top(Vrange___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_init_top\n"); );
    // Body
    Vrange___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vrange___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vrange___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vrange___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vrange___024root__trace_register(Vrange___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vrange___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vrange___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vrange___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vrange___024root__trace_full_sub_0(Vrange___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vrange___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_full_top_0\n"); );
    // Init
    Vrange___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vrange___024root*>(voidSelf);
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vrange___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vrange___024root__trace_full_sub_0(Vrange___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vrange__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrange___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clk));
    bufp->fullBit(oldp+2,(vlSelf->go));
    bufp->fullIData(oldp+3,(vlSelf->start),32);
    bufp->fullBit(oldp+4,(vlSelf->done));
    bufp->fullSData(oldp+5,(vlSelf->count),16);
    bufp->fullCData(oldp+6,((0xfU & vlSelf->start)),4);
    bufp->fullIData(oldp+7,(vlSelf->range__DOT__c1__DOT__current_value),32);
    bufp->fullBit(oldp+8,(vlSelf->range__DOT__c1__DOT__computation_done));
    bufp->fullIData(oldp+9,(0x10U),32);
    bufp->fullIData(oldp+10,(4U),32);
    bufp->fullCData(oldp+11,(0U),4);
    bufp->fullBit(oldp+12,(0U));
    bufp->fullSData(oldp+13,(0U),16);
    bufp->fullSData(oldp+14,(vlSelf->range__DOT__mem[0]),16);
    bufp->fullSData(oldp+15,(vlSelf->range__DOT__mem[1]),16);
    bufp->fullSData(oldp+16,(vlSelf->range__DOT__mem[2]),16);
    bufp->fullSData(oldp+17,(vlSelf->range__DOT__mem[3]),16);
    bufp->fullSData(oldp+18,(vlSelf->range__DOT__mem[4]),16);
    bufp->fullSData(oldp+19,(vlSelf->range__DOT__mem[5]),16);
    bufp->fullSData(oldp+20,(vlSelf->range__DOT__mem[6]),16);
    bufp->fullSData(oldp+21,(vlSelf->range__DOT__mem[7]),16);
    bufp->fullSData(oldp+22,(vlSelf->range__DOT__mem[8]),16);
    bufp->fullSData(oldp+23,(vlSelf->range__DOT__mem[9]),16);
    bufp->fullSData(oldp+24,(vlSelf->range__DOT__mem[10]),16);
    bufp->fullSData(oldp+25,(vlSelf->range__DOT__mem[11]),16);
    bufp->fullSData(oldp+26,(vlSelf->range__DOT__mem[12]),16);
    bufp->fullSData(oldp+27,(vlSelf->range__DOT__mem[13]),16);
    bufp->fullSData(oldp+28,(vlSelf->range__DOT__mem[14]),16);
    bufp->fullSData(oldp+29,(vlSelf->range__DOT__mem[15]),16);
}
