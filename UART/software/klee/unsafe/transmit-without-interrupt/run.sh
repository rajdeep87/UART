#!/bin/bash
clang -emit-llvm -I/home/klee/klee_src/include -g -c nondet-transfer64.c -DKLEE -o nondet-transfer64.bc
klee --libc=uclibc --posix-runtime nondet-transfer.bc
