#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"

/* function declarations for test cases in tests.c */
static void testDeqConstructor();
static void testDeqHeadPut();
static void testDeqTailPut();
static void testDeqHeadGet();
static void testDeqTailGet();
static void testDeqHeadRem();
static void testDeqTailRem();
static void testDeqToString();

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
