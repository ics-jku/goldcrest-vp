#include "stdint.h"

#ifdef X86
#include "stdio.h"
#endif

static volatile int * const PRINT_CHAR_ADDR = (int * const)1024;
static volatile int * const PRINT_INT_ADDR = (int * const)1016;

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

void sort(int *ra, unsigned int n, unsigned int pivot) {
  int i, j;
  int ir = n;
  unsigned int l = pivot; //(n >> 1) + 1;
  int rra;

  for (;;) {
    if (l > 1) {
      rra = ra[--l];
    } else {
      rra = ra[ir];
      ra[ir] = ra[1];
      if (--ir == 1) {
	ra[1] = rra;
	return;
      }
    }

    i = l;
    j = l << 1;
    while (j <= ir) {
      if (j < ir && ra[j] < ra[j+1]) { 
	++j; 
      }
      if (rra < ra[j]) {
	ra[i] = ra[j];
	j += (i = j);
      } else {
	j = ir + 1;
      }
    }
    ra[i] = rra;
  }
}

int main() {
  int test_1[200] = {5
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
  int test_1_size = 200;
  int test_1_pivot = 100;
  sort(test_1, test_1_size, test_1_pivot);
  print(test_1, test_1_size);
  
  return 0;
}
