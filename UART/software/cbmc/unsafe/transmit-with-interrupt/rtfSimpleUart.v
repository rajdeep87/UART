// ============================================================================
//	(C) 2007,2011,2013  Robert Finch
//  All rights reserved.
//	robfinch@<remove>finitron.ca
//
//	rtfSimpleUart.v
//		Basic uart with	baud rate generator based on a harmonic
//	frequency synthesizer.
//
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the <organization> nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//  	To use:
// 
//  	Set the pClkFreq parameter to the frequency of the system
//  	clock (clk_i). This can be done when the core is instanced.
// 
//    1) set the baud rate value in the clock multiplier
//    registers (CM1,2,3). A default multiplier value may
//    be specified using the pClkMul parameter, so it
//    doesn't have to be programmed at run time. (Note the
//    pBaud parameter may also be set, but it doesn't work
//    in all cases due to arithmetic limitations).
//    2) enable communication by activating the rts, and
//    dtr signals in the modem control register. These
//    signals are defaulted to be active on reset, so they
//    may not need to be set. The pRts and pDtr parameters
//    may be used to change the default setting.
//    3) use interrupts or poll the status register to
//    determine when to transmit or receive a byte of data
//    4) read / write the transmit / recieve data buffer
//    for communication.
//
//    Notes:
//    	This core only supports a single transmission /
//    reception format: 1 start, 8 data, and 1 stop bit (no
//    parity).	
//    	The baud rate generator uses a 24 bit harmonic
//    frequency synthesizer. Compute the multiplier value
//    as if a 32 bit value was needed, then take the upper
//    24 bits of the value. (The number of significant bits
//    in the value determine the minimum frequency
//    resolution or the precision of the value).
//
//    				baud rate * 16
//    	value = -----------------------
//    			(clock frequency / 2^32)
//  
//  		eg			38400 * 16
//  		value = -----------------------
//				(28.63636MHz / 2^32)
//				
//				= 92149557.65
//				= 057E1736 (hex)
//				
//				
//		taking the upper 24 bits
//				top 24 = 057E17
//						= 359959
//				
//		so the value needed to be programmed into the register
//	for 38.4k baud is 57E17 (hex)
//		eg 	CM0 = 0 (not used)
//			CM1 = 17 hex
//			CM2 = 7E hex
//			CM3 = 05 hex
//
//
//	Register Description
//
//	reg
//	0	read / write (RW)
//		TRB - transmit / receive buffer
//		transmit / receive buffer
//		write 	- write to transmit buffer
//		read	- read from receive buffer
//
//	1	read only (RO)
//		LS	- line status register
//		bit 0 = receiver not empty, this bit is set if there is
//				any data available in the receiver fifo
//		bit 1 = overrun, this bit is set if receiver overrun occurs
//		bit 3 = framing error, this bit is set if there was a
//				framing error with the current byte in the receiver
//				buffer.
//		bit 5 = transmitter not full, this bit is set if the transmitter
//				can accept more data
//		bit 6 = transmitter empty, this bit is set if the transmitter is
//				completely empty
//
//	2	MS	- modem status register (RO)
//		writing to the modem status register clears the change
//		indicators, which should clear a modem status interrupt
//		bit 3 = change on dcd signal
//		bit 4 = cts signal level
//		bit 5 = dsr signal level
//		bit 6 = ri signal level
//		bit 7 = dcd signal level
//
//	3	IS	- interrupt status register (RO)
//		bit 0-4 = mailbox number
//		bit 0,1	= 00
//		bit 2-4	= encoded interrupt value
//		bit 5-6 = not used, reserved
//		bit 7 = 1 = interrupt pending, 0 = no interrupt
//
//	4	IE	- interrupt enable register (RW)
//		bit 0 = receive interrupt (data present)
//		bit 1 = transmit interrupt (data empty)
//		bit 3 = modem status (dcd) register change
//		bit 4-7 = unused, reserved
//
//	5	FF	- frame format register		(RW)
//		this register doesn't do anything in the simpleUart
//		but is reserved for compatiblity with the more
//		advanced uart
//
//	6	MC	- modem control register (RW)
//		bit 0 = dtr signal level output
//		bit 1 = rts signal level output
//              bit 4 = internal loopback mode
//
//	7	- control register
//		bit 0 = hardware flow control,
//			when this bit is set, the transmitter output is
//			controlled by the cts signal line automatically
//
//
//		* Clock multiplier steps the 16xbaud clock frequency
//		in increments of 1/2^32 of the clk_i input using a
//		harmonic frequency synthesizer
//		eg. to get a 9600 baud 16x clock (153.6 kHz) with a
//		27.175 MHz clock input,
//		value  = upper24(9600 * 16  / (27.175MHz / 2^32))
//		Higher frequency baud rates will exhibit more jitter
//		on the 16x clock, but this will mostly be masked by the 
//		16x clock factor.
//
//	8	CM0	- Clock Multiplier byte 0 (RW)
//		this is the least significant byte
//		of the clock multiplier value
//		this register is not used unless the clock
//		multiplier is set to contain 32 bit values
//
//	9	CM1 - Clock Multiplier byte 1	(RW)
//		this is the third most significant byte
//		of the clock multiplier value
//		this register is not used unless the clock
//		multiplier is set to contain 24 or 32 bit values
//
//	10	CM2 - Clock Multiplier byte 2	(RW)
//		this is the second most significant byte of the clock
//		multiplier value
//
//	11	CM3	- Clock Multiplier byte 3 	(RW)
//		this is the most significant byte of the multiplier value
//
//	12	FC	- Fifo control register		(RW)
//		this register doesnt' do anything in the simpleUart
//		but is reserved for compatibility with the more
//		advanced uart
//		
//	13-14	reserved registers
//
//	15	SPR	- scratch pad register (RW)
//
//
//   	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|WISHBONE Datasheet
//	|WISHBONE SoC Architecture Specification, Revision B.3
//	|
//	|Description:						Specifications:
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|General Description:				simple UART core
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|Supported Cycles:					SLAVE,READ/WRITE
//	|									SLAVE,BLOCK READ/WRITE
//	|									SLAVE,RMW
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|Data port, size:					8 bit
//	|Data port, granularity:			8 bit
//	|Data port, maximum operand size:	8 bit
//	|Data transfer ordering:			Undefined
//	|Data transfer sequencing:			Undefined
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|Clock frequency constraints:		none
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|Supported signal list and			Signal Name		WISHBONE equiv.
//	|cross reference to equivalent		ack_o			ACK_O
//	|WISHBONE signals					adr_i[3:0]		ADR_I()
//	|									clk_i			CLK_I
//	|                                   rst_i           RST_I()
//	|									dat_i(7:0)		DAT_I()
//	|									dat_o(7:0)		DAT_O()
//	|									cyc_i			CYC_I
//	|									stb_i			STB_I
//	|									we_i			WE_I
//	|
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	|Special requirements:
//	+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//=============================================================================

`define UART_TRB    4'd0    // transmit/receive buffer
`define UART_LS     4'd1    // line status register
`define UART_MS     4'd2    // modem status register
`define UART_IS		4'd3	// interrupt status register
`define UART_IER    4'd4    // interrupt enable
`define UART_FF     4'd5    // frame format register
`define UART_MC     4'd6    // modem control register
`define UART_CTRL	4'd7	// control register
`define UART_CLKM0	4'd8	// clock multiplier byte 0
`define UART_CLKM1	4'd9	// clock multiplier byte 1
`define UART_CLKM2	4'd10	// clock multiplier byte 2
`define UART_CLKM3	4'd11	// clock multiplier byte 3
`define UART_FC     4'd12   // fifo control register
`define UART_SPR     4'd15   // scratchpad register (added by joleary)

module rtfSimpleUart(rst_i,clk_i,cyc_i,stb_i,we_i,adr_i,dat_i,dat_o,ack_o,vol_o,irq_o,cts_ni,rts_no,dsr_ni,dcd_ni,dtr_no,rxd_i,txd_o,data_present_o);

	// WISHBONE Slave interface
	input rst_i;		// reset
	input clk_i;		// eg 100.7MHz
	input cyc_i;		// cycle valid
	input stb_i;		// strobe
	input we_i;			// 1 = write
	input [31:0] adr_i;		// register address
	input [7:0] dat_i;		// data input bus
	output [7:0] dat_o;	// data output bus
	output ack_o;		// transfer acknowledge
  reg [7:0] dat_o;	// data output bus
	output vol_o;		// volatile register selected
	output irq_o;		// interrupt request
	//----------------
	input cts_ni;		// clear to send - active low - (flow control)
	output rts_no;	// request to send - active low - (flow control)
	input dsr_ni;		// data set ready - active low
	reg rts_no;	// request to send - active low - (flow control)
	input dcd_ni;		// data carrier detect - active low
	output dtr_no;	// data terminal ready - active low
	input rxd_i;			// serial data in
	reg dtr_no;	// data terminal ready - active low
	output txd_o;			// serial data out
	output data_present_o;

parameter pClkFreq = 20000000;	// clock frequency in MHz
parameter pBaud = 19200;
parameter pClkMul = (4096 * pBaud) / (pClkFreq / 65536);
parameter pRts = 1;		// default to active
parameter pDtr = 1;

wire cs = cyc_i && stb_i && (adr_i[31:4]==28'hFFDC_0A0);
assign ack_o = cs;
assign vol_o = cs && adr_i[3:2]==2'b00;

//-------------------------------------------
// variables
reg [23:0] c;	// current count
reg [23:0] ck_mul;	// baud rate clock multiplier
reg [7:0] spr;
wire tx_empty;
wire baud16;	// edge detector (active one cycle only!)
reg rx_present_ie;
reg tx_empty_ie;
reg dcd_ie;
reg hwfc;			// hardware flow control enable
reg loopback;    // loopback enabled
wire clear = cyc_i && stb_i && we_i && adr_i[3:0]==4'd13;
wire frame_err;		// receiver char framing error
wire over_run;		// receiver over run
reg [1:0] ctsx;		// cts_ni sampling
reg [1:0] dcdx;
reg [1:0] dsrx;
wire dcd_chg = dcdx[1]^dcdx[0];


wire rxIRQ = data_present_o & rx_present_ie;
wire txIRQ = tx_empty & tx_empty_ie;
wire msIRQ = dcd_chg & dcd_ie;

assign irq_o = 
	  rxIRQ
	| txIRQ
	| msIRQ
	;

wire [2:0] irqenc =
	rxIRQ ? 1 :
	txIRQ ? 3 :
	msIRQ ? 4 :
	0;

wire [7:0] rx_do;
wire txrx = cs && adr_i[3:0]==4'd0;

wire txd_int;
wire rxd_int;

// Support for loopback mode
assign rxd_int = loopback ? txd_int : rxd_i;
//RM: assign txd_o = loopback ? 1'b1 : txd_int;
wire cts_nint;
assign cts_nint = loopback ? data_present_o : cts_ni;
/*
wire rts_nint;
wire cts_nint;
assign cts_nint = loopback ? rts_nint : cts_ni;
assign rts_no = loopback ? 1'b1 : rts_nint;
assign dsr_nint = loopback ? dtr_nint : dsr_ni;
assign dtr_no = loopback? 1'b1 : dtr_nint;
JO: dtr_no and rts_no are controlled by two bits in UART_MC
JO: the Tx partition 
JO: How does this work "for real"?
JO: dsr_ni
*/

/*
rtfSimpleUartRx uart_rx0(
	.rst_i(rst_i),
	.clk_i(clk_i),
	.cyc_i(cyc_i),
	.stb_i(stb_i),
	.cs_i(txrx),
	.we_i(we_i),
	.dat_o(rx_do),
	.baud16x_ce(baud16),
	.clear(clear),
	.rxd(rxd_int),
	.data_present(data_present_o),
	.frame_err(frame_err),
	.overrun(over_run)
        // JO: ack_o and baud8x are unconnected.
        , .ack_o()
        , .baud8x(1'b0)
);

rtfSimpleUartTx uart_tx0(
	.rst_i(rst_i),
	.clk_i(clk_i),
	.cyc_i(cyc_i),
	.stb_i(stb_i),
	.cs_i(txrx),
	.we_i(we_i),
	.dat_i(dat_i),
	.baud16x_ce(baud16),
	.cts(ctsx[1]|~hwfc),
	.txd(txd_int),
	.empty(tx_empty)
        // JO unconnected:
        , .ack_o()
        , .baud8x(1'b0)
        , .txc()
);
*/

// mux the reg outputs
always @*
	if (cs) begin
		case(adr_i[3:0])	// synopsys full_case parallel_case
		`UART_LS:	dat_o <= {1'b0, tx_empty, tx_empty, 1'b0, frame_err, 1'b0, over_run, data_present_o};
		`UART_MS:	dat_o <= {dcdx[1],1'b0,dsrx[1],ctsx[1],dcd_chg,3'b0};
		`UART_IS:	dat_o <= {irq_o, 2'b0, irqenc, 2'b0};
                `UART_IER:      dat_o <= {4'b0000, dcd_ie, 1'b0, tx_empty_ie, rx_present_ie};                
                `UART_MC:       dat_o <= {3'b000, loopback, 2'b00, ~rts_no, ~dtr_no};
                `UART_CTRL:     dat_o <= {7'b0000000, hwfc};
                `UART_CLKM0:    dat_o <= 8'h00;
                `UART_CLKM1:    dat_o <= ck_mul[7:0];
                `UART_CLKM2:    dat_o <= ck_mul[15:8];
                `UART_CLKM3:    dat_o <= ck_mul[23:16];
                `UART_SPR:	dat_o <= spr;
		default:	dat_o <= rx_do;
		endcase
	end
	else
		dat_o <= 8'b0;

// Note: baud clock should pulse high for only a single
// cycle!
always @(posedge clk_i)
	if (rst_i)
		c <= 0;
	else
		c <= c + ck_mul;

// for detecting an edge on the msb
//edge_det ed0(.rst(rst_i), .clk(clk_i), .ce(1'b1), .i(c[23]), .pe(baud16), .ne(), .ee() );
   
// register updates
always @(posedge clk_i) begin
	if (rst_i) begin
		rts_no <= ~pRts;
		rx_present_ie <= 1'b0;
		tx_empty_ie <= 1'b0;
		dcd_ie <= 1'b0;
		hwfc <= 1'b1;
		dtr_no <= ~pDtr;
                loopback <= 1'b0;
		ck_mul <= pClkMul;
           	spr <= 8'h00;
	end
	else if (cs & we_i) begin
		case (adr_i[3:0])
		`UART_IER:
				begin
				rx_present_ie <= dat_i[0];
				tx_empty_ie <= dat_i[1];
				dcd_ie <= dat_i[3];
				end
		`UART_MC:
				begin
				dtr_no <= ~dat_i[0];
				rts_no <= ~dat_i[1];
                                loopback <= dat_i[4];
				end
		`UART_CTRL:	hwfc <= dat_i[0];
		`UART_CLKM1:	ck_mul[7:0] <= dat_i;
		`UART_CLKM2:	ck_mul[15:8] <= dat_i;
		`UART_CLKM3:	ck_mul[23:16] <= dat_i;
                `UART_SPR:	spr <= dat_i;
		default:
			;
		endcase
	end    
end


// synchronize external signals
always @(posedge clk_i)
	ctsx <= {ctsx[0],~cts_nint};

always @(posedge clk_i)
	dcdx <= {dcdx[0],~dcd_ni};

always @(posedge clk_i)
	dsrx <= {dsrx[0],~dsr_ni};

endmodule

