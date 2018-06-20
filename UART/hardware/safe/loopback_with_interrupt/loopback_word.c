#include<assert.h>
#include "rtfSimpleUart.h"

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
  //assert(rtfSimpleUart.ack_o == 1);
  // We assume the acknowledge comes right away.
  // NB Wishbone does not guarantee this in general!
  // The simple UART appears to derive ack_o combinatorially from stb_i and cyc_i.
  next_timeframe();
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
  //assert(rtfSimpleUart.ack_o == 1);
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

  // Reset

  wb_reset();
  wb_idle();

  // Configure the uart

  outb (0x13, UART_MC);  // Loopback mode
  outb (0x80, UART_CM3); // Hella big clock multiplier!
  outb (0x00, UART_CM2);
  outb (0x00, UART_CM1);
  outb (0x00, UART_CR);  // no:  hardware flow control
  outb (0x03, UART_IE);  // yes: tx_empty and rx_data interrupts 

  // Now the Uart is configured in loopback mode with both tx_empty and
  // rx_data interrupts enabled. We just have to wait for the first
  // interrupt, which will be a tx interrupt, and start sending and
  // receiving data.

  char tx_b[] = "Raj";
  _u8 istatus = 0;
  char rx_b[100];
  int i;
  int c=0, d=0;

  for (i=0; i<1000; i++) {

    if (rtfSimpleUart.irq_o) {

      istatus = inb(UART_IS) & 0x0c;
      if (istatus == 0x0c) {
        // it was a tx_empty interrupt
        outb(*(tx_b+c), UART_TR);
        c++;
        
      } else { // istatus==0x04
        // it was an rx_data interrupt
        rx_b[d] = inb(UART_TR);
        d++;
      }

    } else {

      // no interrupt. 
      // Note, same number of RTL clocks in each arm of if
      wb_idle();
      wb_idle();
    }
  }

  for(i=0; i<=2; i++)
    assert(rx_b[i] == tx_b[i]);

  return 1;
}
