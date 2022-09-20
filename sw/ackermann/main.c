
#ifdef X86
#include <stdio.h>
#include <stdlib.h>
#else
static volatile int * const PRINT_CHAR_ADDR = (int * const)1024;
static volatile int * const PRINT_INT_ADDR = (int * const)1016;

int putchar(int c) {
  *PRINT_CHAR_ADDR = c;
  return 0;
}

int putint(int i) {
  *PRINT_INT_ADDR = i;
  return 0;
}

void print(char* str) {
  while (*str) *PRINT_CHAR_ADDR = *str++;
}
#endif


int Ack(int M, int N) {
  if (M == 0) return( N + 1 );
  if (N == 0) return( Ack(M - 1, 1) );
  return( Ack(M - 1, Ack(M, (N - 1))) );
}

int main() {
  int n = 6;

  #ifdef X86
  printf("Ack(3,%d): %d\n", n, Ack(3, n));
  #else
  print("Ack(3,");
  putint(n);
  print("): ");
  putint(Ack(3, n));
  print("\n");
  #endif
  return 0;
}
