#include <stdio.h>

int a = 19 ;
const char*.str = "%d";
int add_some_value() {
  return 10;
}

int main() {
  int c = add_some_value();
  auto d = a + c;
  printf(.str, d);
  return 0;
}
