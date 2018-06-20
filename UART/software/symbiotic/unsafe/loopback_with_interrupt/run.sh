#!/bin/bash
FLAGS=-DKLEE ../Symbiotic/symbiotic --debug=prepare --64 --optimize=before-O2,after --no-prepare --no-slice loopback64.c

