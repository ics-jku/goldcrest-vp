#include "stdint.h"

#ifdef X86
#include "stdio.h"
#endif

static volatile int * const PRINT_CHAR_ADDR = (int * const)1024;
static volatile int * const PRINT_INT_ADDR = (int * const)1016;

/* void print(char* str) { */
/*   int32_t* c; */
/*   //for (int i = -14; i < 0; i++) { */
/*   c = (int32_t*) str; */
/*   while (c != 0) { */
/*   c = (int32_t*) str;   */
/*     *PRINT_CHAR_ADDR = *c;         */
/*     str++; */
/*   } */
/* } */

void print(int* data, unsigned int size) {
    for (int i = 0; i != size; i++) {
      #ifdef X86
      printf("%d ", data[i]);
      #else
      *PRINT_INT_ADDR = data[i];
      *PRINT_CHAR_ADDR = ' ';
      #endif
    }
    #ifdef X86
    printf("\n");
    #else
    *PRINT_CHAR_ADDR = '\n';
    #endif
}

void sort(int* data, unsigned int size) {
  int swapped = 1;

  while (swapped) {
    //print(data, size);
    swapped = 0;
    for (int i = 1; i != size; i++) {
      if (data[i] <= data[i-1]) {
	int tmp = data[i-1];
	data[i-1] = data[i];
	data[i] = tmp;
	if (data[i] != data[i-1]) swapped = 1;
      }
    }
  }

  print(data, size);
}

int main() {
  /* int test_0[18] = {47, 99, 8, 87, 100, 50, 43, 71, 36, 98, 26, 77, 85, 89, 46, 94, 1, 16}; */
  /* int test_0_size = 18; */
  /* sort(test_0, test_0_size); */


  int test_0[200] = {5
    , 27, 5, 83, 71, 91, 27, 62, 78, 31, 57, 33, 29, 50, 62, 93, 21, 59, 41, 70, 96
    , 92, 47, 52, 10, 59, 75, 69, 10, 10, 64, 48, 12, 3, 52, 35, 79, 89, 73, 11, 77
    , 26, 45, 33, 23, 36, 74, 80, 72, 8, 76, 68, 20, 27, 54, 42, 97, 38, 70, 46, 85
    , 56, 42, 63, 38, 34, 38, 96, 39, 65, 70, 76, 62, 94, 87, 87, 84, 74, 92, 11, 13
    , 100, 84, 24, 23, 37, 33, 51, 58, 81, 99, 68, 75, 55, 75, 92, 10, 76, 54, 14, 80
    , 39, 100, 80, 89, 62, 11, 14, 61, 91, 45, 53, 99, 42, 13, 48, 66, 0, 75, 20, 78
    , 74, 92, 28, 80, 16, 21, 19, 59, 79, 93, 50, 28, 87, 90, 0, 72, 16, 82, 16, 51
    , 33, 94, 52, 2, 75, 33, 35, 37, 15, 28, 31, 77, 50, 21, 61, 80, 12, 73, 67, 74
    , 58, 37, 38, 4, 81, 61, 30, 83, 3, 87, 75, 3, 75, 78, 39, 1, 80, 95, 94, 6
    , 46, 79, 92, 66, 40, 61, 52, 28, 100, 36, 26, 60, 76, 98, 17, 56, 48, 5, 71};
  int test_0_size = 200;
  sort(test_0, test_0_size);


  
  return 0;
}
