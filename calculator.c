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
   uint64_t fract : 52;
   int bias;
} binary64;

void bin(uint64_t n) {
  uint64_t i;
  for(i = 1ULL << 63; i > 0; i = i / 2)
  (n & i) ? printf("1") : printf("0");
}

int main() {
  uint64_t val;
  printf("input number: ");
  scanf("%lf", &val);

  uint64_t val2;
  memcpy(&val2, &val, sizeof(uint64_t));

  bin(val2);
  printf("\n");
  // printf("%llu \n", val2);
  // printf("%llu \n", val);

  binary64.bias = 1023;
  binary64.sign = val2 >> 63;
  binary64.exp = (val2 >> 52) & 0x7FF; //11 1's
  binary64.fract = val2 & 0xffffffffffffffff; //52 1's 54

  /*bin(val2 >> 1);
  printf("\n");*/
  printf("%llu \n", val2 & 0xfffffffffffff);
  bin(0b0100000010001111000010000000000000000000000000000000000000000000 & 0xfffffffffffff);
  printf("\n");

  binary32.bias = 127;
  binary32.sign = binary64.sign;
  binary32.exp = binary64.exp - binary64.bias + binary32.bias;
  binary32.fract = val2 & 0x7fffff; //23 1's 0x7fffff

  binary16.bias = 15;
  binary16.sign = binary32.sign;
  binary16.exp = binary32.exp -binary32.bias + binary16.bias;
  binary16.fract = binary32.fract >> 13;

  printf("binary64: sign: %d, exp: %lu, fraq: %llu \n", binary64.sign, binary64.exp, binary64.fract);
  printf("binary32: sign: %d, exp: %lu, fraq: %llu \n", binary32.sign, binary32.exp, binary32.fract);
  printf("binary16: sign: %d, exp: %lu, fraq: %llu \n", binary16.sign, binary16.exp, binary16.fract);
  

  return 0;
}
