#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct {
   uint16_t sign : 1;
   uint16_t exp : 5;
   uint16_t fract : 10;
   int bias;
} binary16;

struct {
   uint32_t sign : 1;
   uint32_t exp : 8;
   uint32_t fract : 23;
   int bias;
} binary32;

struct {
   uint16_t sign : 1;
   uint64_t exp : 11;
   uint64_t fract;
   int bias;
} binary64;

void bin(unsigned n) {
  unsigned i;
  for(i = 1 << 31; i > 0; i = i / 2)
  (n & i) ? printf("1") : printf("0");
}

int main() {
  float val;
  printf("input number: ");
  scanf("%f", &val);

  uint32_t val2;
  memcpy(&val2, &val, sizeof(uint32_t));

  binary32.bias = 127;
  binary32.sign = val2 >> 31;
  binary32.exp = (val2 >> 23) & 0b11111111;
  binary32.fract = val2 & 0b11111111111111111111111;

  binary16.bias = 15;
  binary16.sign = binary32.sign;
  binary16.exp = binary32.exp -binary32.bias + binary16.bias;
  binary16.fract = binary32.fract >> 13;

  binary64.bias = 1023;
  binary64.sign = binary32.sign;
  binary64.exp = binary32.exp -binary32.bias + binary64.bias;
  binary64.fract = binary32.fract;

  printf("binary32: sign: %d, exp: %d, fraq: %d \n", binary32.sign, binary32.exp, binary32.fract);
  printf("binary16: sign: %d, exp: %d, fraq: %d \n", binary16.sign, binary16.exp, binary16.fract);
  printf("binary64: sign: %d, exp: %d, fraq: %lu", binary64.sign, binary64.exp, binary64.fract);

  return 0;
}
