#include <math.h>
#include <stdio.h>

int main() {
  double i = 0;
  while(i < 5)
  {
    printf("{%f} double i, {%f} round i\n",i, round(i));
    i += 0.1;
  }
  return 0;
}