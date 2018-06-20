#include<assert.h>
#include <math.h>
#include "rtfSimpleUart.h"

unsigned char nondet_uchar();


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

int main(void) {
 wb_reset();
 wb_idle();
 wb_idle();
 wb_idle();
 wb_idle();
 assert(0);
}

