// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vcollatz.h"
#include "Vcollatz__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vcollatz::Vcollatz(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vcollatz__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , go{vlSymsp->TOP.go}
    , done{vlSymsp->TOP.done}
    , n{vlSymsp->TOP.n}
    , dout{vlSymsp->TOP.dout}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vcollatz::Vcollatz(const char* _vcname__)
    : Vcollatz(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vcollatz::~Vcollatz() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vcollatz___024root___eval_debug_assertions(Vcollatz___024root* vlSelf);
#endif  // VL_DEBUG
void Vcollatz___024root___eval_static(Vcollatz___024root* vlSelf);
void Vcollatz___024root___eval_initial(Vcollatz___024root* vlSelf);
void Vcollatz___024root___eval_settle(Vcollatz___024root* vlSelf);
void Vcollatz___024root___eval(Vcollatz___024root* vlSelf);

void Vcollatz::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vcollatz::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vcollatz___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vcollatz___024root___eval_static(&(vlSymsp->TOP));
        Vcollatz___024root___eval_initial(&(vlSymsp->TOP));
        Vcollatz___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vcollatz___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vcollatz::eventsPending() { return false; }

uint64_t Vcollatz::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vcollatz::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vcollatz___024root___eval_final(Vcollatz___024root* vlSelf);

VL_ATTR_COLD void Vcollatz::final() {
    Vcollatz___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vcollatz::hierName() const { return vlSymsp->name(); }
const char* Vcollatz::modelName() const { return "Vcollatz"; }
unsigned Vcollatz::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> Vcollatz::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vcollatz___024root__trace_init_top(Vcollatz___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vcollatz___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcollatz___024root*>(voidSelf);
    Vcollatz__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    Vcollatz___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void Vcollatz___024root__trace_register(Vcollatz___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vcollatz::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vcollatz::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vcollatz___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
