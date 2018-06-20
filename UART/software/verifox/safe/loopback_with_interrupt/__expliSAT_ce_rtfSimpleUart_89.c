#include <formal.h>
#include <assert.h>


unsigned long nondet_ulong()
{
   assert(0);
   return 0;
}

signed long nondet_long()
{
   assert(0);
   return 0;
}

unsigned int nondet_uint()
{
   assert(0);
   return 0;
}

signed int nondet_int()
{
   assert(0);
   return 0;
}

unsigned short nondet_ushort()
{
   assert(0);
   return 0;
}

signed short nondet_short()
{
   assert(0);
   return 0;
}

unsigned char nondet_uchar()
{
   assert(0);
   return 0;
}

signed char nondet_char()
{
   assert(0);
   return 0;
}

void fv_initToNondet(void* arg0)
{
}

int fv_checkBufferOverflow(unsigned long long arg0)
{
   static int counter = 0;
   int value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

void fv_endAtomicBlock()
{
}

void fv_beginAtomicBlock()
{
}

unsigned long fv_getAllocationSize()
{
   static int counter = 0;
   unsigned long value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

unsigned int fv_getBufferSize(const void* arg0)
{
   static int counter = 0;
   unsigned int value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

int fv_hint(int arg0)
{
   static int counter = 0;
   int value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

void fv_dummy()
{
}

void fv_printState()
{
}

int fv_isAlive(void* arg0)
{
   static int counter = 0;
   int value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

void fv_print(unsigned long long arg0)
{
}

int fv_hasConcreteValue(unsigned long long arg0)
{
   static int counter = 0;
   int value;
   switch (counter) {
      default: assert(0);
   }
   counter += 1;
   return value;
}

void fv_assume(int condition)
{
}

void fv_assert(int condition)
{
   assert(condition);
}

