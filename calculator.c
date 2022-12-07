#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct {
   uint16_t sign : 1;
   uint64_t exp : 5;
   uint64_t fract : 10;
   int bias;
} binary16;

struct {
   uint16_t sign : 1;
   uint64_t exp : 8;
   uint64_t fract : 23;
   int bias;
} binary32;

struct {
   uint16_t sign : 1;
   uint64_t exp : 11;
   uint64_t fract : 52;
   int bias;
} binary64;

char* bin(uint64_t n) {
  uint64_t i;
  int j = 0;
  char *str = malloc(65);
  bool zeroes = true;

  for(i = 1ULL << 63; i > 0; i = i / 2) {
   if(n & i) {
      str[j]='1';
      zeroes = false;
   } else {
      if(zeroes) continue;
      str[j]='0';
   }
   j++;
  }
   str[j] = '\0';
   return str;
}

int main() {
  double val;
  printf("input number: ");
  scanf("%lf", &val);

  uint64_t val2;
  memcpy(&val2, &val, sizeof(uint64_t));

  binary64.bias = 1023;
  binary64.sign = val2 >> 63;
  binary64.exp = (val2 >> 52) & 0x7FF; //11 1's
  binary64.fract = val2 & 0xffffffffffffffff; //52 1's 54
  
  if(val > 3.4028234664e38) {
    printf("binary32 overflow. Result is +inf \n");
  } else if(val < -3.4028234664e38) {
    printf("binary32 underflow. Result is -inf \n");
  } 

  binary32.bias = 127;
  binary32.sign = binary64.sign;
  binary32.exp = binary64.exp - binary64.bias + binary32.bias;
  binary32.fract = (binary64.fract >> 29) & 0x7fffff; //23 1's 0x7fffff

  if(val > 65504) {
    printf("binary16 overflow. Result is +inf \n");
  } else if(val < -65504) {
    printf("binary16 underflow. Result is -inf \n");
  } else if(val < 0.000060975552){
    printf("binary16 is subnormal \n");
  }

  binary16.bias = 15;
  binary16.sign = binary32.sign;
  binary16.exp = binary32.exp -binary32.bias + binary16.bias;
  binary16.fract = binary32.fract >> 13 & 0b1111111111;

  printf("binary64: sign: %d, exp: %011s, fraq: %052s \n", binary64.sign, bin(binary64.exp), bin(binary64.fract));
  printf("binary32: sign: %d, exp: %08s, fraq: %023s \n", binary32.sign, bin(binary32.exp), bin(binary32.fract));
  printf("binary16: sign: %d, exp: %05s, fraq: %010s \n", binary16.sign, bin(binary16.exp), bin(binary16.fract));

  return 0;
}
