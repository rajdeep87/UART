#include <stdio.h>
#include "rtfSimpleUartTx.c"
#include "rtfSimpleUartRx.c"
#include "edge_det.c"

#define TRUE 1
#define FALSE 0

#define UART_TRB    0     // transmit/receive buffer
#define UART_LS      1    // line status register
#define UART_MS     2     // modem status register
#define UART_IS		  3	    // interrupt status register
#define UART_IER    4     // interrupt enable
#define UART_FF     5     // frame format register
#define UART_MC     6     // modem control register
#define UART_CTRL	  7	    // control register
#define UART_CLKM0	8	    // clock multiplier byte 0
#define UART_CLKM1	9	    // clock multiplier byte 1
#define UART_CLKM2	10	  // clock multiplier byte 2
#define UART_CLKM3	11	  // clock multiplier byte 3
#define UART_FC     12    // fifo control register
#define UART_SPR    15    // special purpose register


struct state_elements_rtfSimpleUart {
  // registers
  _Bool hwfc;
  unsigned char ctsx;
  unsigned int c;
  unsigned char dcdx;
  unsigned int ck_mul;
  unsigned char dsrx;
  _Bool rx_present_ie;
  _Bool tx_empty_ie;
  _Bool dcd_ie;
  
  // extra added registers
  _Bool loopback; 
  unsigned char spr;
  
  // output register 
  _Bool rts_no;
  unsigned char dat_o;
  _Bool dtr_no;
};
struct state_elements_rtfSimpleUart  srtfSimpleUart;

struct diagnosem {
  _Bool baud16;
};
struct diagnosem dm;


void rtfSimpleUart(_Bool rst_i, _Bool clk_i, _Bool cyc_i, _Bool stb_i, _Bool we_i, unsigned int adr_i, unsigned char dat_i, unsigned char *dat_o, _Bool *ack_o, _Bool *vol_o, _Bool *irq_o, _Bool cts_ni, _Bool *rts_no, _Bool dsr_ni, _Bool dcd_ni, _Bool *dtr_no, _Bool rxd_i, _Bool *txd_o, _Bool *data_present_o)
{
  // shadow variables declaration
  unsigned char tmp37;
  unsigned char tmp36;
  unsigned char tmp35;
  unsigned char tmp34;
  unsigned char tmp33;
  unsigned char tmp32;
  unsigned char tmp31;
  unsigned char tmp30;
  unsigned char tmp29;
  unsigned char tmp28;
  unsigned char tmp27;
  unsigned char tmp26;
  unsigned char tmp25;
  unsigned char tmp24;
  unsigned char tmp23;
  unsigned char tmp22;
  unsigned char tmp21;
  unsigned char tmp20;
  _Bool tmp19;
  _Bool tmp18;
  _Bool tmp17;
  _Bool tmp16;
  _Bool tmp15;
  _Bool tmp14;
  unsigned int tmp13;
  _Bool tmp12;
  _Bool tmp11;
  _Bool tmp10;
  _Bool tmp9;
  _Bool tmp8;
  _Bool tmp7;
  unsigned int tmp6;
  unsigned int tmp5;
  unsigned char tmp4;
  unsigned char tmp3;
  unsigned char tmp2;
  unsigned char tmp1;
  unsigned char tmp0;
  _Bool pe;
  _Bool ne;
  _Bool ee;
  _Bool flag = 0; 
  // parameter declaration
  int pClkFreq;
  pClkFreq = 20000000;
  int pBaud;
  pBaud = 19200;
  int pClkMul;
  pClkMul =  (4096 * pBaud) / (pClkFreq / 65536);
  int pRts;
  pRts = 1;
  int pDtr;
  pDtr = 1;

  // wire declarations
  _Bool cs;
  _Bool tx_empty;
  _Bool baud16;
  _Bool clear;
  _Bool frame_err;
  _Bool over_run;
  _Bool dcd_chg;
  _Bool rxIRQ;
  _Bool txIRQ;
  _Bool msIRQ;
  unsigned char irqenc;
  unsigned char rx_do;
  _Bool txrx;
  
  // extra wires
  _Bool txd_int;
  _Bool rxd_int;
  _Bool cts_nint;


  // Collect all the assignment statement together
  // cs is assigned combinatorially
  cs = cyc_i && stb_i && (((adr_i >> 4) & 0xFFFFFFF) == 0xFFDC0A0);
  *ack_o = cs;
  *vol_o = cs && (((adr_i >> 2) & 0xF) == 0);
  clear = cyc_i && stb_i && we_i && ((adr_i & 0xF) == 13);
  dcd_chg = ((srtfSimpleUart.dcdx >> 1) & 0x1) ^ (srtfSimpleUart.dcdx & 0x1);

  
  rxIRQ = *data_present_o && srtfSimpleUart.rx_present_ie;
  txIRQ = tx_empty && srtfSimpleUart.tx_empty_ie;
  msIRQ = dcd_chg && srtfSimpleUart.dcd_ie;
  *irq_o = rxIRQ || txIRQ || msIRQ;
  irqenc = rxIRQ ? 1 : (txIRQ ? 3 : (msIRQ ? 4 : 0));
  txrx = cs && ((adr_i & 0xF) == 0);
  
  // Support for loopback modes
  rxd_int = srtfSimpleUart.loopback ? txd_int : rxd_i;
  *txd_o = srtfSimpleUart.loopback ? 1 : txd_int;
  cts_nint = srtfSimpleUart.loopback ? *data_present_o : cts_ni;
  // end of assignment statements

  
  
  if(cs)
  {
    if((adr_i & 0xF) == UART_LS) {
      tmp2 = (0 << 7) | (srtfSimpleUartTx.empty << 6) | (srtfSimpleUartTx.empty << 5) | (0 << 4) | (srtfSimpleUartRx.frame_err << 3) | (0 << 2) | (srtfSimpleUartRx.overrun << 1) | *data_present_o;
     // for the first time shipping of data, following assertions must hold, PASSED
     /*assert(srtfSimpleUartTx.empty == 1);
     assert(srtfSimpleUartRx.frame_err == 0);
     assert(srtfSimpleUartRx.overrun == 0);
     assert(*data_present_o == 0);
     printf("The data read is tmp2=%x",tmp2);
     assert(tmp2 == 0x60); */
  }
    else if((adr_i & 0xF) == UART_MS)
      tmp0 = tmp0 | (((srtfSimpleUart.dcdx >> 1) & 0x1) << 7) | (0 << 6) | (((srtfSimpleUart.dsrx >> 1) & 0x1) << 5) | (((srtfSimpleUart.ctsx >> 1) & 0x1) << 4) | (dcd_chg << 3) | 0 & 0x7;

    else if((adr_i & 0xF) == UART_IS)
      tmp1 = tmp1 | (*irq_o << 7) | (0 << 0x3) << 5 | (irqenc && 0x7) << 2 | 0 & 0x3;
    
    else if((adr_i & 0xF) == UART_IER)
      tmp26 = tmp26 | (0 & 0xF << 4) | (srtfSimpleUart.dcd_ie << 3) | (1 << 2) | (srtfSimpleUart.tx_empty_ie << 1) | (srtfSimpleUart.rx_present_ie);
     
    else if((adr_i & 0xF) == UART_MC)
      tmp27 = tmp27 | (0 & 0x7 << 4) | (srtfSimpleUart.loopback << 3) | (0 & 0x3 << 2) | !(srtfSimpleUart.rts_no << 1) | !(srtfSimpleUart.dtr_no);     
    
    else if((adr_i & 0xF) == UART_CTRL)
      tmp28 = tmp28 | ((0 & 0x7F) << 1) | (srtfSimpleUart.hwfc);
    
    else if((adr_i & 0xF) == UART_CLKM0)
      tmp29 = 0 & 0xFF;
      
    else if((adr_i & 0xF) == UART_CLKM1)
      tmp30 = srtfSimpleUart.ck_mul & 0xFF;
      
    else if((adr_i & 0xF) == UART_CLKM2)
      tmp31 = (((srtfSimpleUart.ck_mul >> 8)) & 0xFF);
    
    else if((adr_i & 0xF) == UART_CLKM3)
      tmp32 = ((srtfSimpleUart.ck_mul >> 16) & 0xFF);
    
    else if((adr_i & 0xF) == UART_SPR)
      tmp33 = srtfSimpleUart.spr & 0xFF;
    
  // default case
    else
      tmp3 = rx_do;
  }
  else
    tmp4 = 0;
  
  
  // Note: baud clock should pulse high for only a single
  // cycle!

  if(rst_i)
    tmp5 = 0;
  else
    tmp6 = srtfSimpleUart.c + srtfSimpleUart.ck_mul;

  // register updates
  if(rst_i)
  {
    tmp7 = !pRts;
    tmp8 = FALSE;
    tmp9 = FALSE;
    tmp10 = FALSE;
    tmp11 = TRUE;
    tmp12 = !pDtr;
    tmp34 = 0; // extra assignment for loopback
    tmp13 = pClkMul;
    tmp35 = 0; // extra assignment for spr
  }
  else
    if(cs && we_i)
    {
      if((adr_i & 0xF) == UART_IER)
      {
        tmp14 = dat_i & 0x1;
        tmp15 = ((dat_i >> 1) & 0x1);
        tmp16 = ((dat_i >> 3) & 0x1);
      }
      else if((adr_i & 0xF) == UART_MC)
      {
        tmp17 = !dat_i & 0x1;
        tmp18 = ((!dat_i >> 1) & 0x1);
        tmp36 = ((dat_i >> 4) & 1); // extra assignment for loopback
      }

      else if((adr_i & 0xF) == UART_CTRL) {
        printf("Hello!, I am doing hardware flow control");
        flag=1;
        tmp19 = ((dat_i >> 0) & 0x1);
      }  
      else if((adr_i & 0xF) == UART_CLKM1)
        tmp20 = dat_i & 0xFF;

      else if((adr_i & 0xF) == UART_CLKM2)
        tmp21 = dat_i & 0xFF;

      else if((adr_i & 0xF) == UART_CLKM3) {
        tmp22 = dat_i & 0xFF;
        // print some statistics
        printf("tmp22 is %x\n", tmp22);
      }
      else if((adr_i & 0xF) == UART_SPR)
        tmp37 = dat_i & 0xFF; // extra assignment for loopback
      
      else
      {
        // no assignment  
      }
    }
 
   
  // synchronize external signals
  tmp23 = ((srtfSimpleUart.ctsx & 0x1) << 1)| !cts_nint;
  tmp24 = ((srtfSimpleUart.dcdx & 0x1) << 1) | !dcd_ni;
  tmp25 = ((srtfSimpleUart.dsrx & 0x1) << 1) | !dsr_ni;


   // actual assignments to registers
 
   if(cs) {
   if((adr_i & 0xF) == UART_LS) {
      *dat_o = tmp2;
      srtfSimpleUart.dat_o = tmp2;
      
      //the following assertion must hold when doing the first inb(LS);
      /*assert(srtfSimpleUart.dat_o == 0x60);
      assert(*dat_o == 0x60);
      printf("I am checking Line Ststus\n");*/
   }
   
   else if((adr_i & 0xF) == UART_MS) {
      *dat_o = tmp0;
      srtfSimpleUart.dat_o = tmp0;
   }
       
   else if((adr_i & 0xF) == UART_IS) {
      *dat_o = tmp1;
      srtfSimpleUart.dat_o = tmp1;
   }
    
   else if((adr_i & 0xF) == UART_IER) {
      *dat_o = tmp26;
      srtfSimpleUart.dat_o = tmp26;
   }
   
   else if((adr_i & 0xF) == UART_MC) {
      *dat_o = tmp27;
      srtfSimpleUart.dat_o = tmp27;
   }
   
   else if((adr_i & 0xF) == UART_CTRL) {
      *dat_o = tmp28;
      srtfSimpleUart.dat_o = tmp28;
   }
  
   else if((adr_i & 0xF) == UART_CLKM0) {
      *dat_o = tmp29;
      srtfSimpleUart.dat_o = tmp29;
   }
            
   else if((adr_i & 0xF) == UART_CLKM1) {
      *dat_o = tmp30;
      srtfSimpleUart.dat_o = tmp30;
   }
      
   else if((adr_i & 0xF) == UART_CLKM2) {
      *dat_o = tmp31;
      srtfSimpleUart.dat_o = tmp31;
   }
   
   else if((adr_i & 0xF) == UART_CLKM3) {
      *dat_o = tmp32;
      srtfSimpleUart.dat_o = tmp32;
   }
    
   else if((adr_i & 0xF) == UART_SPR) {
      *dat_o = tmp33;
      srtfSimpleUart.dat_o = tmp33;
   }
      
   // default case
    else {
      *dat_o = tmp3;
      srtfSimpleUart.dat_o = tmp3;
    }
  }
  else
    tmp4 = 0;
  
  
  // Note: baud clock should pulse high for only a single
  // cycle!
  if(rst_i)
    srtfSimpleUart.c = tmp5;
  else
    srtfSimpleUart.c = tmp6;
  


  //register updates 
  if(rst_i)
  {
    srtfSimpleUart.rts_no = tmp7;
    *rts_no = tmp7;
    srtfSimpleUart.rx_present_ie = tmp8;
    srtfSimpleUart.tx_empty_ie = tmp9;
    srtfSimpleUart.dcd_ie = tmp10;
    srtfSimpleUart.hwfc = tmp11;
    srtfSimpleUart.dtr_no = tmp12;
    *dtr_no = tmp12;
    srtfSimpleUart.loopback = tmp34; // extra assignments
    srtfSimpleUart.ck_mul = tmp13;
    srtfSimpleUart.spr = tmp35; // extra assignments
  }
  else
    if(cs && we_i)
    {
      if((adr_i & 0xF) == UART_IER)
      {
        srtfSimpleUart.rx_present_ie = tmp14;
        srtfSimpleUart.tx_empty_ie = tmp15;
        srtfSimpleUart.dcd_ie = tmp16;
      }

      if((adr_i & 0xF) == UART_MC)
      {
        srtfSimpleUart.dtr_no = tmp17;
        *dtr_no = tmp17;
        srtfSimpleUart.rts_no = tmp18;
        *rts_no = tmp18;
        srtfSimpleUart.loopback = tmp36; // extra assignment for loopback
      }

      if((adr_i & 0xF) == UART_CTRL)
      { flag=1;
        srtfSimpleUart.hwfc = tmp19;}

      if((adr_i & 0xF) == UART_CLKM1)
        srtfSimpleUart.ck_mul = (srtfSimpleUart.ck_mul & 0xffff00) | (tmp20 & 0x0000ff);

      if((adr_i & 0xF) == UART_CLKM2)
        srtfSimpleUart.ck_mul = (srtfSimpleUart.ck_mul & 0xff00ff) | ((tmp21 << 8)  & 0x00ff00);

      if((adr_i & 0xF) == UART_CLKM3) {
        //printf("The clk multiplier value in this module is %x\n",srtfSimpleUart.ck_mul);
        srtfSimpleUart.ck_mul = (srtfSimpleUart.ck_mul & 0x00ffff) | ((tmp22 << 16) & 0xff0000);
      }
      if((adr_i & 0xF) == UART_SPR)
        srtfSimpleUart.spr = tmp37;
      else
      {}
    }

   srtfSimpleUart.ctsx = tmp23;
   srtfSimpleUart.dcdx = tmp24;
   srtfSimpleUart.dsrx = tmp25;

  // Collect all the assignment statement together
  //cs = cyc_i && stb_i && (((adr_i >> 4) & 0xFFFFFFF) == 0xFFDC0A0);
  //*ack_o = cs;
  
  // check this assertions holds during any outb
  if(!rst_i && cyc_i && stb_i) {
    assert(cs == 1);
    assert(*ack_o == 1);
    // following assertion only holds when outb(UART_IE) is called
    //assert((adr_i & 0xF) == 4);
    // following assertion only holds when outb(UART_CR) is called
    //assert((adr_i & 0xF) == 7);
    //assert(flag == 1);
  }
  
   // check that when IE is LOW, then the following must hold
   if(!rst_i && cs && we_i)
   {
     if((adr_i & 0xF) == 4)
     {
        assert(srtfSimpleUart.rx_present_ie == (dat_i & 1));
        assert(srtfSimpleUart.tx_empty_ie == ((dat_i >> 1) & 1));
        assert(srtfSimpleUart.dcd_ie == ((dat_i >> 3) & 1));
     }
   }
  // check the property of flow control
   if(!rst_i && cs && we_i)
   {
     if((adr_i & 0xF) == UART_CTRL)
     {
       assert(srtfSimpleUart.hwfc == (dat_i & 1));
     }
   }

  // check the device reset configurations
  if(rst_i)
  {
    // assert the top module signal configuration
    assert(srtfSimpleUart.rx_present_ie == 0);
    assert(srtfSimpleUart.tx_empty_ie == 0);
    assert(srtfSimpleUart.dcd_ie == 0);
    assert(srtfSimpleUart.hwfc == 1);
    assert(srtfSimpleUart.loopback == 0);
    assert(srtfSimpleUart.spr == 0);
  }
  
  // call to edge detector
  _Bool i = ((srtfSimpleUart.c >> 23) & 1);
  _Bool val=1;
  edge_det(rst_i, clk_i, val, i, &baud16, &ne, &ee);
  printf("the baud rate is baud16= %d\n", baud16); 
  // diagnose 
  dm.baud16 = baud16;
  // call to receiver
  unsigned char temp0 = 0;
  rtfSimpleUartRx(rst_i, clk_i, cyc_i, stb_i, ack_o, we_i, &rx_do, txrx, baud16, temp0, clear, rxd_int, data_present_o, &frame_err, &over_run);
  
  
  // call to transmitter
  _Bool txc;
  _Bool temp1 = ((srtfSimpleUart.ctsx >> 1) & 1) | ~srtfSimpleUart.hwfc;
  rtfSimpleUartTx(rst_i, clk_i, cyc_i, stb_i, ack_o, we_i, dat_i, txrx, baud16, 0, temp1, &txd_int, &tx_empty, &txc);

}

// Main function
/*void main() {
 while(1) {
  _Bool rst_i;_Bool clk_i;_Bool cyc_i;_Bool stb_i; _Bool we_i;
  unsigned int adr_i; unsigned char dat_i;
  unsigned char dat_o; 
  _Bool ack_o; _Bool vol_o; _Bool irq_o; _Bool cts_ni; 
  _Bool rts_no; _Bool dsr_ni; _Bool dcd_ni; _Bool dtr_no; 
  _Bool rxd_i; _Bool txd_o; _Bool data_present_o;

  rtfSimpleUart(rst_i, clk_i, cyc_i, stb_i, we_i, adr_i, dat_i, &dat_o, &ack_o, &vol_o, &irq_o, cts_ni, &rts_no, dsr_ni, dcd_ni, &dtr_no, rxd_i, &txd_o, &data_present_o);

 }
}*/
