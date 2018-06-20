TOP= rtfSimpleUart
VERILOG_FILES= $(TOP).v rtfSimpleUart.v rtfSimpleUartTx.v rtfSimpleUartRx.v edge_det.v

loopback64: loopback64.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback64.c $(VERILOG_FILES) --module $(TOP) --bound 2700 --aig --vcd $(TOP).vcd

loopback32: loopback32.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback32.c $(VERILOG_FILES) --module $(TOP) --aig --bound 1400 --vcd $(TOP).vcd

loopback16: loopback16.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback16.c $(VERILOG_FILES) --module $(TOP) --bound 1100 --aig --vcd $(TOP).vcd

loopback8: loopback8.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback8.c $(VERILOG_FILES) --module $(TOP) --bound 500 --aig --vcd $(TOP).vcd

loopback_array: loopback_array.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback_array.c $(VERILOG_FILES) --module $(TOP) --bound 4000 --vcd $(TOP).vcd

loopback_ar_small: loopback_array_small.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback_array_small.c $(VERILOG_FILES) --module $(TOP) --bound 2200 --vcd $(TOP).vcd

debug: $(TOP).vcd
	gtkwave $(TOP).vcd waves.gtkw

$(TOP).h: $(TOP).v
	hw-cbmc $(VERILOG_FILES) --module $(TOP) --gen-interface | sed -n '/Unwinding Bound/,$$p' > $(TOP).h

clean:
	rm -f $(TOP).h $(TOP).vcd
