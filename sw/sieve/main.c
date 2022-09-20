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

int
main(int argc, char *argv[]) {
  int NUM = 100;
  static char flags[8192 + 1];
  long i, k;
  int count = 0;

  while (NUM--) {
    count = 0; 
    for (i=2; i <= 8192; i++) {
      flags[i] = 1;
    }
    for (i=2; i <= 8192; i++) {
      if (flags[i]) {
	/* remove all multiples of prime: i */
	for (k=i+i; k <= 8192; k+=i) {
	  flags[k] = 0;
	}
	count++;
      }
    }
  }

#ifdef X86
  printf("Count: %d\n", count);
#else
  print("Count: ");
  putint(count);
  print("\n");
#endif

  return(0);
}
