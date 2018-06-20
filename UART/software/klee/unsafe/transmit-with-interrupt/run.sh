#!/bin/bash
clang -emit-llvm -I/home/klee/klee_src/include -g -c transmit_interrupt64.c -DKLEE -o transmit_interrup64.bc
klee --libc=uclibc --posix-runtime transmit_interrupt.bc
