#!/bin/bash
clang -emit-llvm -I/home/klee/klee_src/include -g -c loopback64.c -DKLEE -o loopback64.bc
klee --libc=uclibc --posix-runtime loopback64.bc
