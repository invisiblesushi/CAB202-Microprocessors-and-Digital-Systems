#include <stdio.h>
#include <math.h>



int main ()
{
   double start_x, start_y, x, y, ret, val;

   start_x = 1;
   start_y = 1;

   x = 10;
   y = 10;

   x = x - start_x;
   y = y - start_y;
   val = 180.0 / PI;

   ret = atan2 (y,x) * val;
   printf("The arc tangent of x = %lf, y = %lf ", x, y);
   printf("is %lf degrees\n", ret);

   return(0);
}
