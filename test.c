#include <math.h>
#include <stdio.h>

int main() {
  int i = -90;
  while(i++ < 90)
  {
    printf("{%d}int i, {%f}\n",i, tan((i*3.14)/180));
  }
  return 0;
}