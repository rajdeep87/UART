TOP= rtfSimpleUart
VERILOG_FILES= $(TOP).v rtfSimpleUart.v rtfSimpleUartTx.v rtfSimpleUartRx.v edge_det.v

harness: harness.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc harness.c $(VERILOG_FILES) --module $(TOP) --bound 520 --vcd $(TOP).vcd

transmit-interrupt64: transmit_interrupt64.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc transmit_interrupt64.c $(VERILOG_FILES) --module $(TOP) --bound 500 --aig --vcd $(TOP).vcd

transmit-interrupt32: transmit_interrupt32.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc transmit_interrupt32.c $(VERILOG_FILES) --module $(TOP) --bound 250 --aig --vcd $(TOP).vcd

test: firmware.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc firmware.c $(VERILOG_FILES) --module $(TOP) --bound 10 --aig --vcd $(TOP).vcd

debug: $(TOP).vcd
	gtkwave $(TOP).vcd waves.gtkw

$(TOP).h: $(TOP).v
	hw-cbmc $(VERILOG_FILES) --module $(TOP) --gen-interface | sed -n '/Unwinding Bound/,$$p' > $(TOP).h

clean:
	rm -f $(TOP).h $(TOP).vcd
