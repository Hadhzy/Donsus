#include <stdio.h>

int func(int a, int b) {
  int e = a + b;
  return e;
}
int main(){
  auto d = func(12, 12);
  printf("%d", d);
  return 0;
}