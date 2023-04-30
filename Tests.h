#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "deq.h"

void testDeqConstructor();
void testDeqHeadPut();
void testDeqTailPut();
void testDeqHeadGet();
void testDeqTailGet();
void testDeqHeadRem();
void testDeqTailRem();
void testDeqToString();

static void runTests() {
  testDeqConstructor();
  testDeqHeadPut();
  testDeqTailPut();
  testDeqHeadGet();
  testDeqTailGet();
  testDeqHeadRem();
  testDeqTailRem();
  testDeqToString();
}
