// ENTRY POINT (main function)
#include "../Include/runner.h"
#include <iostream>

#define VERSION_STRING "0.0.1"

const std::string USAGE_TEXT = "TBD";

void print_usage() { std::cout << USAGE_TEXT << std::endl; }

int main(int argc, char const **argv) {
  if (argc <= 1)
    print_usage();
  return Du_Main(argc, argv);
}
