#include <stdio.h>
#define TRUE 1
#define FALSE 0

#define IDLE 0
#define CNT 1
#define CNT_FRAME 0x97
#define CNT_FINISH 0x9D

// Structure declaration for storing state holding elements
struct state_elements_rtfSimpleUartRx{
  unsigned char rxdd;
  _Bool rxdsmp;
  _Bool rdxstart;
  unsigned char rxdsum;
  _Bool data_present;
  _Bool frame_err;
  _Bool overrun;
  unsigned char cnt;
  unsigned short int rx_data;
  _Bool state;
  _Bool wf;
  unsigned char dat;
  _Bool modeX8;
};
struct state_elements_rtfSimpleUartRx  srtfSimpleUartRx;

void rtfSimpleUartRx(_Bool rst_i, _Bool clk_i, _Bool cyc_i, _Bool stb_i, _Bool *ack_o, _Bool we_i, unsigned char *dat_o, 
                     _Bool cs_i, _Bool baud16x_ce, _Bool baud8x, _Bool clear, _Bool rxd, _Bool *data_present, _Bool *frame_err, _Bool *overrun)
{
  _Bool tmp30;
  unsigned char tmp29;
  _Bool tmp28;
  unsigned char tmp27;
  unsigned short int tmp26;
  _Bool tmp25;
  _Bool tmp24;
  _Bool tmp23;
  _Bool tmp22;
  _Bool tmp21;
  _Bool tmp20;
  _Bool tmp19;
  _Bool tmp18;
  _Bool tmp17;
  _Bool tmp16;
  _Bool tmp15;
  _Bool tmp14;
  _Bool tmp13;
  _Bool tmp12;
  _Bool tmp11;
  _Bool tmp10;
  _Bool tmp9;
  _Bool tmp8;
  _Bool tmp7;
  _Bool tmp6;
  _Bool tmp5;
  unsigned char tmp4;
  _Bool tmp3;
  _Bool tmp2;
  _Bool tmp1;
  unsigned char tmp0;
  
  // parameter declaration
  int SamplerStyle;
  SamplerStyle = 0;

  _Bool isX8;
  unsigned int sum;
  
  // update data register
  if(srtfSimpleUartRx.wf)
   tmp0 = ((srtfSimpleUartRx.rx_data >> 1) & 0xFF);

  // on a read clear the data present status
  // but set the status when the data register
  // is updated by the receiver		
  if(rst_i)
    tmp1 = FALSE;
  else
    if(srtfSimpleUartRx.wf)
      tmp2 = TRUE;
  else
    if(*ack_o && !we_i)
        tmp3 = FALSE;

  
  // Three stage synchronizer to synchronize incoming data to
  // the local clock (avoids metastability).
  tmp4 = (((srtfSimpleUartRx.rxdd >> 0) & 31) << 1) | (rxd);
  
  if(SamplerStyle == 0)
  {
    tmp5 = ((srtfSimpleUartRx.rxdd >> 3) & 1);
    tmp6 = ((srtfSimpleUartRx.rxdd >> 4) & 1) & (!((srtfSimpleUartRx.rxdd >> 3) & 1));
  }

  else
  {
    tmp7 = (srtfSimpleUartRx.rxdsum & 1);
    sum = 0 << 1 | ((srtfSimpleUartRx.rxdd >> 3) & 1) + ((0 << 1) | ((srtfSimpleUartRx.rxdd >> 4) & 1)) + ((0 << 1) | ((srtfSimpleUartRx.rxdd >> 5) & 1)); 
    tmp8 = sum;
    tmp9 = ((srtfSimpleUartRx.rxdsum >> 1) & 1);
    tmp10 = (((srtfSimpleUartRx.rxdsum >> 1) & 1) == 0) & (((srtfSimpleUartRx.rxdsum >> 1) & 1) == 3);
  }
  
  
  if(rst_i)
  {
    tmp11 = IDLE;
    tmp12 = FALSE;
    tmp13 = FALSE;
    tmp14 = FALSE;
  }
  else
  {
    // clear the write flag
    tmp15 = FALSE;
    if(clear)
    {
      tmp16 = FALSE;
      tmp17 = IDLE;
      tmp18 = FALSE;
      tmp19 = FALSE;
    }
    else
      if(baud16x_ce)
      {
        if((unsigned int)srtfSimpleUartRx.state == IDLE)
        {
          if(srtfSimpleUartRx.rdxstart)
            tmp20 = CNT;
        }
        if((unsigned int)srtfSimpleUartRx.state == CNT)
        {
          if(srtfSimpleUartRx.cnt == CNT_FRAME)
          {
            tmp21 = !srtfSimpleUartRx.rxdsmp;
            tmp22 = srtfSimpleUartRx.data_present;
            if(!srtfSimpleUartRx.data_present)
              tmp23 = TRUE;
            tmp24 = IDLE;
          }

          // On start bit check make sure the start
					// bit is low, otherwise go back to the
					// idle state because it's a false start.
          if(srtfSimpleUartRx.cnt == 7 && srtfSimpleUartRx.rxdsmp)
            tmp25 = IDLE;

          if(srtfSimpleUartRx.cnt & 0xF == 7)
            tmp26 = ((srtfSimpleUartRx.rxdsmp << 9) | ((srtfSimpleUartRx.rx_data >> 1 ) & 511));
        }
      }
  }
  
  // bit rate counter
  if(baud16x_ce)
  {
    if(srtfSimpleUartRx.state == IDLE)
    {
      tmp27 = srtfSimpleUartRx.modeX8;
      tmp28 = isX8;
    }
    else
    {
      tmp29 = ((srtfSimpleUartRx.cnt >> 1) & 0x7F) + ((srtfSimpleUartRx.cnt >> 0) & 1);
      tmp30 = !((srtfSimpleUartRx.cnt >> 0) & 1) | (srtfSimpleUartRx.modeX8 & 1);
    }
  }

  // Now handle all non-blocking assignments
  if(srtfSimpleUartRx.wf)
    srtfSimpleUartRx.dat = tmp0;

  if(rst_i)
  {
    srtfSimpleUartRx.data_present = tmp1;
    *data_present = tmp1;
  }
  else
    if(srtfSimpleUartRx.wf)
    {
      srtfSimpleUartRx.data_present = tmp2;
      *data_present = tmp2;
    }
    else
      if(*ack_o && !we_i)
      {
        srtfSimpleUartRx.data_present = tmp3;
        *data_present = tmp3;
      }

  srtfSimpleUartRx.rxdd = tmp4;
  
  if(SamplerStyle == 0)
  {
    srtfSimpleUartRx.rxdsmp = tmp5;
    srtfSimpleUartRx.rdxstart = tmp6;
  }
  else
  {
    srtfSimpleUartRx.rxdsum = srtfSimpleUartRx.rxdsum | (tmp7 << 1);
    srtfSimpleUartRx.rxdsum = srtfSimpleUartRx.rxdsum | tmp8;
    srtfSimpleUartRx.rxdsmp = tmp9;
    srtfSimpleUartRx.rdxstart = tmp10;
  }

  if(rst_i)
  {
    srtfSimpleUartRx.state = tmp11;
    srtfSimpleUartRx.wf = tmp12;
    srtfSimpleUartRx.overrun = tmp13;
    *overrun = tmp13;
    srtfSimpleUartRx.frame_err = tmp14;
    *frame_err = tmp14;
  }
  else
  {
    srtfSimpleUartRx.wf = tmp15;
    if(clear)
    {
      srtfSimpleUartRx.wf = tmp16;
      srtfSimpleUartRx.state = tmp17;
      srtfSimpleUartRx.overrun = tmp18;
      *overrun = tmp18;
      srtfSimpleUartRx.frame_err = tmp19;
      *frame_err = tmp19;
    }
    else
      if(baud16x_ce)
      {
        if((unsigned int)srtfSimpleUartRx.state == 0)
        {
          if(srtfSimpleUartRx.rdxstart)
            srtfSimpleUartRx.state = tmp20;
        }
        if((unsigned int)srtfSimpleUartRx.state == 1)
        {
          if(srtfSimpleUartRx.cnt == 151)
          {
            srtfSimpleUartRx.frame_err = tmp21;
            *frame_err = tmp21;
            srtfSimpleUartRx.overrun = tmp22;
            *overrun = tmp22;
            if(!srtfSimpleUartRx.data_present)
              srtfSimpleUartRx.wf = tmp23;
            srtfSimpleUartRx.state = tmp24;
           }
           if((srtfSimpleUartRx.cnt == 7) && srtfSimpleUartRx.rxdsmp)
             srtfSimpleUartRx.state = tmp25;

           if((srtfSimpleUartRx.cnt & 0xF) == 7)
             srtfSimpleUartRx.rx_data = tmp26;
         }
       }
  }
  
  if(baud16x_ce)
  {
    if((unsigned int)srtfSimpleUartRx.state == 0)
    {
      srtfSimpleUartRx.cnt = tmp27;
      srtfSimpleUartRx.modeX8 = tmp28;
    }

    else
    {
      srtfSimpleUartRx.cnt = srtfSimpleUartRx.cnt | ((tmp29 << 1) & 0x7F);
      srtfSimpleUartRx.cnt = srtfSimpleUartRx.cnt | (tmp30 & 1);
    }
  }

  // Handle all assignment statements
  isX8 = baud8x;
  *ack_o = cyc_i && stb_i && cs_i;
  *dat_o = *ack_o ? srtfSimpleUartRx.dat : 0;

  // check the device configuration when reset
  // assert the top module signal configuration
  /*if(rst_i) {
    assert(data_present == 0);
    assert(srtfSimpleUartRx.state == 0);
    assert(srtfSimpleUartRx.wf == 0);
    assert(srtfSimpleUartRx.overrun == 0);
    assert(*overrun == 0);
    assert(srtfSimpleUartRx.frame_err == 0);
    assert(*frame_err == 0);
  }*/

}

