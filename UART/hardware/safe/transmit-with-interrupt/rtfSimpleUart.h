/* Unwinding Bound */

extern const unsigned int bound;

/* Next Timeframe  */

void next_timeframe(void);
void set_inputs(void);

// type declarations
typedef signed int _u32;
typedef unsigned __CPROVER_bitvector[24] _u24;
typedef unsigned char _u8;
typedef unsigned __CPROVER_bitvector[2] _u2;
typedef unsigned __CPROVER_bitvector[3] _u3;

// Module Verilog::rtfSimpleUart

struct module_rtfSimpleUart {
  _u32 pClkFreq;
  _u32 pBaud;
  _u32 pClkMul;
  _u32 pRts;
  _u32 pDtr;
  _Bool cs;
  _u24 c;
  _u24 ck_mul;
  _u8 spr;
  _Bool tx_empty;
  _Bool baud16;
  _Bool rx_present_ie;
  _Bool tx_empty_ie;
  _Bool dcd_ie;
  _Bool hwfc;
  _Bool loopback;
  _Bool clear;
  _Bool frame_err;
  _Bool over_run;
  _u2 ctsx;
  _u2 dcdx;
  _u2 dsrx;
  _Bool dcd_chg;
  _Bool rxIRQ;
  _Bool txIRQ;
  _Bool msIRQ;
  _u3 irqenc;
  _u8 rx_do;
  _Bool txrx;
  _Bool txd_int;
  _Bool rxd_int;
  _Bool cts_nint;
  _Bool rst_i;
  _Bool clk_i;
  _Bool cyc_i;
  _Bool stb_i;
  _Bool we_i;
  _u32 adr_i;
  _u8 dat_i;
  _u8 dat_o;
  _Bool ack_o;
  _Bool vol_o;
  _Bool irq_o;
  _Bool cts_ni;
  _Bool rts_no;
  _Bool dsr_ni;
  _Bool dcd_ni;
  _Bool dtr_no;
  _Bool rxd_i;
  _Bool txd_o;
  _Bool data_present_o;
};



// top module
extern struct module_rtfSimpleUart rtfSimpleUart;

