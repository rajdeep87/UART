#include<assert.h>
#include <math.h>
#include "rtfSimpleUart.h"

unsigned char nondet_uchar();

// ---------------------------------------------------------------------
// Transactions on the wishbone interface
// ---------------------------------------------------------------------

void wb_reset(void) {
  rtfSimpleUart.rst_i = 1;
  set_inputs();
  next_timeframe();
  rtfSimpleUart.rst_i = 0;
  // Rule 3.20
  rtfSimpleUart.stb_i = 0; rtfSimpleUart.cyc_i = 0;
}

void wb_idle() {
  set_inputs();
  next_timeframe();
}

void wb_write(_u32 addr, _u8 b) {
  // Master presents address, data, asserts WE, CYC and STB
  rtfSimpleUart.adr_i = addr;
  rtfSimpleUart.dat_i = b;
  rtfSimpleUart.we_i = 1;
  rtfSimpleUart.cyc_i = 1;
  rtfSimpleUart.stb_i = 1;
  set_inputs();
  // This holds because cs is set combinatorially
  assert(rtfSimpleUart.cs == 1);
  
  // check for UART_IE
  if((rtfSimpleUart.adr_i & 0xF) == 4) {
   assert(((rtfSimpleUart.adr_i >> 4) & 0xFFFFFFF) == 0xFFDC0A0);  
   assert((rtfSimpleUart.adr_i & 0xF) == 4);  
  }
  // We assume the acknowledge comes right away.
  // NB Wishbone does not guarantee this in general!
  // The simple UART appears to derive ack_o combinatorially from stb_i and cyc_i.
  
  next_timeframe();
  
  
  // check for UART_IE
  if((rtfSimpleUart.adr_i & 0xF) == 4) {
   assert(rtfSimpleUart.rx_present_ie == (b & 1));
   assert(rtfSimpleUart.tx_empty_ie == ((b >> 1) & 1));
   assert(rtfSimpleUart.dcd_ie == ((b >> 3) & 1));
  }

  rtfSimpleUart.we_i = 0;
  rtfSimpleUart.cyc_i = 0;
  rtfSimpleUart.stb_i = 0;
}

_u8 wb_read(_u32 addr) {
  // Master presents address, data, asserts CYC and STB, deasserts WE
  rtfSimpleUart.adr_i = addr;
  rtfSimpleUart.we_i = 0;
  rtfSimpleUart.cyc_i = 1;
  rtfSimpleUart.stb_i = 1;
  set_inputs();
  // We assume the acknowledge comes right away.
  // NB Wishbone does not guarantee this in general!
  // The simple UART appears to derive ack_o combinatorially from stb_i and cyc_i.
  _u8 b = rtfSimpleUart.dat_o;
  next_timeframe();
  rtfSimpleUart.we_i = 0;
  rtfSimpleUart.cyc_i = 0;
  rtfSimpleUart.stb_i = 0;
  return b;
}

// ---------------------------------------------------------------------
// Linux-style inb, outb
//
// Right now, these call wb_read and wb_write directly.
//
// If/when we decide to run HW and FW in separate threads,
// inb/outb would execute in the FW thread, wb_read/wb_write would
// execute in the hardware thread, and communication between them
// would be via synchronization or fifo channel.
// ---------------------------------------------------------------------

typedef unsigned char u8;

unsigned char inb (unsigned long port) {
  return wb_read(port);
}

void outb (u8 value, unsigned long port) {
  wb_write(port, value);
}

int power(int base, int exp)
{
  int result = 1;
  while(exp) { result *= base; exp--; }
  return result;
}


// ---------------------------------------------------------------------
// UART Firmware
// ---------------------------------------------------------------------

// UART addresses
// Some of these are not implemented yet in the opencores UART.
#define UART_TR 0xffdc0a00         // tx/rx data (RW)
#define UART_LS (UART_TR + 1)      // line status (RO)
#define UART_MS (UART_TR + 2)      // modem status (RO)
#define UART_IS (UART_TR + 3)      // interrupt status (RO)
#define UART_IE (UART_TR + 4)      // interrupt enable (RW)
#define UART_FF (UART_TR + 5)      // frame format (RW)
#define UART_MC (UART_TR + 6)      // modem control (RW)
#define UART_CR (UART_TR + 7)      // uart control (RW)
#define UART_CM0 (UART_TR + 8)     // clock multiplier byte 0 - least significant (RW)
#define UART_CM1 (UART_TR + 9)     //                  byte 1
#define UART_CM2 (UART_TR + 10)    //                  byte 2
#define UART_CM3 (UART_TR + 11)    //                  byte 3 - most significant (RW)
#define UART_FC (UART_TR + 12)     // fifo control (RW)
#define UART_SPR (UART_TR + 15)    // scratchpad (RW)

// ---------------------------------------------------------------------
// Main test routine
// ---------------------------------------------------------------------

int main(void) {

  _u8 b;
  unsigned int iter=0;
  wb_reset();
  wb_idle();
  // turn off the interrupts
  outb(0x0, UART_IE);
  
  wb_idle();
  // turn off hardware flow control
  outb(0x0, UART_CR);
  assert(rtfSimpleUart.hwfc == 0);
  wb_idle();
  
  // Use a really large clk multiplier! (UART_CM0 is ignored)
  
  assert(rtfSimpleUart.ck_mul != 0);
  
  outb (0x80, UART_CM3);
  assert(rtfSimpleUart.ck_mul == 0x80ef36);
  
  outb (0x00, UART_CM2);
  assert(rtfSimpleUart.ck_mul == 0x800036);
  
  outb (0x00, UART_CM1);
  assert(rtfSimpleUart.ck_mul == 0x800000);

  // *****************************************
  // ship out a byte through the serial port
  // *****************************************
  // check the Line Status register
  //while(iter++ <= 10) {
  unsigned int data_out=0;
  _Bool data_array[8] = {0,0,0,0,0,0,0,0};
  _u8 data = nondet_uchar();
  __CPROVER_assume(data <= 127);
  b = inb(UART_LS); 
  // check the data
  assert(b == 0x60);
  // check other status signals
  assert(rtfSimpleUart.tx_empty == 1);
  assert(rtfSimpleUart.uart_tx0.empty == 1);
  assert(rtfSimpleUart.frame_err == 0);
  assert(rtfSimpleUart.over_run == 0);
  assert(rtfSimpleUart.data_present_o == 0);

  assert(b & 0x40); // tx empty
  assert(!rtfSimpleUart.dtr_no); // data terminal ready
  // ship the data
  outb(data, UART_TR);
  b = inb(UART_LS);
  // check the data, transmitter buffer must be full now
  assert(~b & 0x20); // tx full
  assert(rtfSimpleUart.uart_tx0.fdo == data);

  // following assertions must hold at this point
  /*assert(rtfSimpleUart.baud16 == 1);
  assert(rtfSimpleUart.uart_tx0.baud16x_ce == 1);
  assert(rtfSimpleUart.uart_tx0.tx_data != 0);
  assert(rtfSimpleUart.uart_tx0.cnt == 0x9f);
  assert(rtfSimpleUart.uart_tx0.empty == 0);
  assert(rtfSimpleUart.uart_tx0.cts == 1);
  assert(!rtfSimpleUart.dtr_no); // data terminal ready
  */

  // wait for 100 clock cyles for the data to be transmitted
  // precisely, tx_empty becomes 1 at 14th clock cycle
  for (int i = 0; i < 20; i++) {
   wb_idle();
  }
  
  // transmitter must be empty here
  assert(rtfSimpleUart.uart_tx0.empty == 1);
  b = inb(UART_LS);
  assert(b && 0x20); // tx empty

  // this holds even after 400 cycles (8*32=256 cycles require for serial output)
  int k = 0;
  _Bool flip = 1;
  for (int i = 0; i < 317; i++) {
    if(((rtfSimpleUart.uart_tx0.cnt & 0xF) == 0xF) && (rtfSimpleUart.uart_tx0.fdo == data) && flip==1) {
      data_array[k] = rtfSimpleUart.uart_tx0.txd;  
      k++;
    }
    if(flip == 1)
      flip = 0;
    else { flip = 1; } 
    wb_idle();
  }
  
  //check the data out in the serial port
  _Bool dat;
  for(int i=1;i<=7;i++) {
    dat = data_array[i]; 
    data_out = data_out + (dat * power(2,(i-1))); 
  }
  assert(data_out == data);
 //} // end of while
 return 1;
} //end of main

