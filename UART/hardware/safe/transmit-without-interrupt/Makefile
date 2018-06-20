TOP= rtfSimpleUart
VERILOG_FILES= $(TOP).v rtfSimpleUart.v rtfSimpleUartTx.v rtfSimpleUartRx.v edge_det.v

nondet-transfer32: nondet_transfer32.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc nondet_transfer32.c $(VERILOG_FILES) --module $(TOP) --aig --bound 280 --vcd $(TOP).vcd

nondet-transfer64: nondet_transfer64.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc nondet_transfer64.c $(VERILOG_FILES) --module $(TOP) --aig --bound 520 --vcd $(TOP).vcd

nondet-multi: nondet_multi_transfer64.c $(VERILOG_FILES) $(TOP).h
	/bin/rm -f $(TOP).vcd
	hw-cbmc nondet_multi_transfer64.c $(VERILOG_FILES) --module $(TOP) --bound 5200 --vcd $(TOP).vcd

debug: $(TOP).vcd
	gtkwave $(TOP).vcd waves.gtkw

$(TOP).h: $(TOP).v
	hw-cbmc $(VERILOG_FILES) --module $(TOP) --gen-interface | sed -n '/Unwinding Bound/,$$p' > $(TOP).h

clean:
	rm -f $(TOP).h $(TOP).vcd
