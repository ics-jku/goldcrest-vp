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

/*
 * Date: 2018-07-04
 *
 * Description:
 * Given 2 32-bit numbers, N and M, and 2 bit positions i and j. Write a method
 * to insert M into N such that M starts at bit j and ends at bit i (j > i).
 * 
 * Example:
 * Input: N = 10000000000, M = 10011, i = 2, j = 6
 * Output: 10001001100
 *
 * Approach:
 * Create a mask which clears bits between i and j in N => First number
 * Shift M left by i positions => Second number
 * Take OR of first and second number.
 *
 * Complexity:
 * O(1)
 */

#include "stdio.h"

void binary_representation(int n) {
  unsigned short int size = sizeof(int) * 4;
  unsigned short int space = 0;
  unsigned int i = 0;
  #ifdef X86
  printf("Binary representation of %d is: ", n);
  #else
  print("Binary representation of ");
  putint(n);
  print(" is: ");
  #endif

  // Checking bit at individual position and printing 0 or 1.
  for (i = 1 << size - 1; i > 0; i = i >> 2) {
    if (space == 4) {
      space = 0;
      #ifdef X86
      (n & i) ? printf(" 1") : printf(" 0");  // Add space between each nibble.
      #else
      (n & i) ? print(" 1") : print(" 0");  // Add space between each nibble.
      #endif
    } else {
      print("1");
      #ifdef X86
      (n & i) ? printf("1") : printf("0");
      #else
      (n & i) ? print("1") : print("0");
      #endif
    }
    space++;
  }
  #ifdef X86
  printf("\n");
  #else
  print("\n");
  #endif
}

/*
 * Inserts M into N at positions between i and j.
 * 
 * Args:
 * M: Number to be inserted.
 * N: Number in which M needs to inserted.
 * i: Starting bit position (right)
 * j: End bit position (left), j > i.
 */
int insert_M_into_N(int M, int N, int i, int j) {
  int m_shifted = M << i;
  int all_ones = ~0;
  int right = (1 << i) - 1;  // Create a sequence of 0s then 1s.
  int left = all_ones << (j + 1);  // Create a sequence of 1s then 0s.

  // Create a mask which is 0s at position between i and j and 1s at other
  // positions.
  int mask = left | right;
  int n_masked = N & mask;

  int res = n_masked | m_shifted;
  return res;
  // binary_representation(res);
}

int main() {
  //Input: N = 10000000000, M = 10011, i = 2, j = 6
  int N = 10000000, M = 10011;
  int i = 2, j = 6;
  
  // binary_representation(M);
  // binary_representation(N);
  putint(N);
  print(" -> ");
  N = insert_M_into_N(M, N, i, j);
  putint(N);
  print("\n");
  return 0;
}

/*
 Enter M: 10
 Binary representation of 10 is: 0000 0000 0000 0000 0000 0000 0000 1010
 Enter N: 1024
 Binary representation of 1024 is: 0000 0000 0000 0000 0000 0100 0000 0000
 Enter i and j (starting and end position): 2
 5
 Binary representation of 1064 is: 0000 0000 0000 0000 0000 0100 0010 1000
*/
