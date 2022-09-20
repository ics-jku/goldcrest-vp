#ifdef X86
#include <stdio.h>
#else
static volatile int * const PRINT_CHAR_ADDR = (int * const)1024;

int putchar(int c) {
  *PRINT_CHAR_ADDR = c;
  return 0;
}
#endif

#define PROBLEM_SIZE 4
int main() {
    // output size
    int rows = 40, cols = 78;
    // number of iterations before we quit
    int maxiter = 8;

    int slowdown = PROBLEM_SIZE;

    // size and position of the rect on the imaginary plane
    double fViewRectReal = -2.5, fViewRectImg = -1.0;
    double fMagLevel = 0.05;

    for (int y=0; y < rows; y++)
    {
        // imaginary component of "c"
        double fCImg = fViewRectImg + y * fMagLevel;
        
        for (int x=0; x < cols; x++)
        {
            // real component of "c"
            int bInside;
            int n;
           
            for (int SLOW = 0; SLOW < slowdown; ++SLOW) {
            double fCReal = fViewRectReal + x * fMagLevel;
            double fZReal = fCReal;
            double fZImg = fCImg;
              bInside = 1;
 
              // apply the formula...
              for (n=0; n < maxiter; n++) {
                double fZRealSquared = fZReal * fZReal;
                double fZImgSquared = fZImg * fZImg;

                // have we escaped?
                if (fZRealSquared + fZImgSquared > 4) {
                    bInside = 0;
                    break;
                }

                // z = z^2 + c
                fZImg = 2 * fZReal * fZImg + fCImg;
                fZReal = fZRealSquared - fZImgSquared + fCReal;
              }
            }

/* NOTE: This has been disabled, due to fp-precision sensitivity.  This test 
   is effectively now just a performance test, not a correctness test. */
            /* if (n < 100) */
            /*     putchar('X'); */
            /* else */
            if (bInside)
                putchar(' ');
            else if (n > 3)
	      putchar('~');	    
            else if (n > 2)
                putchar('.');
            else if (n > 1)
                putchar('+');
            else
                putchar('*');
        }
        putchar('\n');
    }
}
