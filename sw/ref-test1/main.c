#include "stdint.h"
#include "stdio.h"

// int x=1, y=1, m=5039;
int x=1, y=1, m=539; 
int c=0, ctr=1, t; 
int xzero=0, yzero=0; 
 

static volatile int * const PRINT_CHAR_ADDR = (int * const)1024;
static volatile int * const PRINT_INT_ADDR = (int * const)1016;

void print(char* str) {
  while (*str != 0) {
    *PRINT_CHAR_ADDR = *str;
    str++;
  }
}
 

int main() { 

  while(1) { 
    y += x; y += y; x += x; 
    while( x>=m ) x-=m; 
    while( y>=m ) y-=m; 
 
    if( x==xzero && y==yzero ) break; 
 
    if ( ++c==ctr ) { 
      xzero=x; yzero=y; 
      c=0; ctr+=ctr; 
    } 
  }

  #ifdef X86
  printf("point: %d %d  loop: %d of %d\n",xzero,yzero,c+1,ctr);
  #else
  print("point: ");
  *PRINT_INT_ADDR = xzero;
  print(" ");
  *PRINT_INT_ADDR = yzero;
  print("  loop: ");
  *PRINT_INT_ADDR = c + 1;
  print(" of ");
  *PRINT_INT_ADDR = ctr;
  print("\n");
  #endif
  
} 
