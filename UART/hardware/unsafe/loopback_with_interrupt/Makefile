TOP= rtfSimpleUart
VERILOG_FILES= $(TOP).v rtfSimpleUart.v rtfSimpleUartTx.v rtfSimpleUartRx.v edge_det.v

loopback64: loopback64.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc loopback64.c $(VERILOG_FILES) --module $(TOP) --bound 2700 --aig --vcd $(TOP).vcd

debug: $(TOP).vcd
	gtkwave $(TOP).vcd waves.gtkw

$(TOP).h: $(TOP).v
	hw-cbmc $(VERILOG_FILES) --module $(TOP) --gen-interface | sed -n '/Unwinding Bound/,$$p' > $(TOP).h

clean:
	rm -f $(TOP).h $(TOP).vcd
