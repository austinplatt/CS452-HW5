#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"

/*
 * Main function that runs all the test cases
 */
int main() {
  testDeqConstructor();
  testDeqHeadPut();
  testDeqTailPut();
  testDeqHeadGet();
  testDeqTailGet();
  testDeqHeadRem();
  testDeqTailRem();
  testDeqToString();
  printf("All tests passed!\n");
  return 0;
}
