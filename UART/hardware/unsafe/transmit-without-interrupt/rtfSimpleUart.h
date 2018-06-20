/* Unwinding Bound */

extern const unsigned int bound;

/* Next Timeframe  */

void next_timeframe(void);
void set_inputs(void);

// type declarations
typedef unsigned int _u32;
typedef unsigned char _u8;
typedef unsigned __CPROVER_bitvector[10] _u10;
typedef unsigned __CPROVER_bitvector[6] _u6;
typedef unsigned __CPROVER_bitvector[2] _u2;
typedef unsigned __CPROVER_bitvector[24] _u24;
typedef unsigned __CPROVER_bitvector[3] _u3;

// Module Verilog::rtfSimpleUartRx

struct module_rtfSimpleUartRx {
  _u32 SamplerStyle;
  _u8 cnt;
  _u10 rx_data;
  _Bool state;
  _Bool wf;
  _u8 dat;
  _Bool isX8;
  _Bool modeX8;
  _u6 rxdd;
  _Bool rxdsmp;
  _Bool rdxstart;
  _u2 rxdsum;
  _Bool rst_i;
  _Bool clk_i;
  _Bool cyc_i;
  _Bool stb_i;
  _Bool ack_o;
  _Bool we_i;
  _u8 dat_o;
  _Bool cs_i;
  _Bool baud16x_ce;
  _Bool baud8x;
  _Bool clear;
  _Bool rxd;
  _Bool data_present;
  _Bool frame_err;
  _Bool overrun;
  //_u8 dat_0;
  //_u6 rxdd_1;
  //_Bool rxdsmp_2;
  //_Bool rdxstart_3;
  /*_u2 rxdsum_4;
  _u2 rxdsum_5;
  _Bool rxdsmp_6;
  _Bool rdxstart_7;
  _Bool frame_err_8;
  _u10 rx_data_9;
  _u8 cnt_10;
  _u8 cnt_11;
  _u8 cnt_12;*/
};

// Module Verilog::rtfSimpleUartTx

struct module_rtfSimpleUartTx {
  _u10 tx_data;
  _u8 fdo;
  _u8 cnt;
  _Bool rd;
  _Bool isX8;
  _Bool modeX8;
  _Bool rst_i;
  _Bool clk_i;
  _Bool cyc_i;
  _Bool stb_i;
  _Bool ack_o;
  _Bool we_i;
  _u8 dat_i;
  _Bool cs_i;
  _Bool baud16x_ce;
  _Bool baud8x;
  _Bool cts;
  _Bool txd;
  _Bool empty;
  _Bool txc;
  /*_u10 tx_data_0;
  _u8 cnt_1;
  _u8 cnt_2;
  _u8 cnt_3;
  _u10 tx_data_4;*/
};

// Module Verilog::edge_det

struct module_edge_det {
  _Bool rst;
  _Bool clk;
  _Bool ce;
  _Bool i;
  _Bool pe;
  _Bool ne;
  _Bool ee;
  _Bool ed;
};

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
  struct module_rtfSimpleUartRx uart_rx0;
  struct module_rtfSimpleUartTx uart_tx0;
  struct module_edge_det ed0;
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
  /*_u8 dat_o_0;
  _u8 dat_o_1;
  _u8 dat_o_2;
  _u8 dat_o_3;
  _u8 dat_o_4;
  _u8 dat_o_5;
  _u8 dat_o_6;
  _u8 dat_o_7;
  _u8 dat_o_8;
  _u24 c_9;
  _Bool rts_no_10;
  _Bool dtr_no_11;
  _u24 ck_mul_12;
  _Bool rx_present_ie_13;
  _Bool tx_empty_ie_14;
  _Bool dcd_ie_15;
  _Bool dtr_no_16;
  _Bool rts_no_17;
  _Bool loopback_18;
  _Bool hwfc_19;
  _u24 ck_mul_20;
  _u24 ck_mul_21;
  _u24 ck_mul_22;
  _u2 ctsx_23;
  _u2 dcdx_24;
  _u2 dsrx_25;*/
};



// top module
extern struct module_rtfSimpleUart rtfSimpleUart;

