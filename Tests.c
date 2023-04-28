#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"

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

/*
 * This is a test case for Deq constructor
 */
static void testDeqConstructor() {
  Deq q = deq_new();
  if (deq_len(q) == 0) {
    printf("Pass: Deq constructor\n");
  } else {
    printf("Fail: Deq constructor\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for Deq head put
 */
static void testDeqHeadPut() {
  Deq q = deq_new();
  int num1 = 1;
  int *ptr1 = &num1;
  deq_head_put(q, ptr1);
  int num2 = 2;
  int *ptr2 = &num2;
  deq_head_put(q, ptr2);
  if (deq_len(q) == 2 && deq_head_get(q) == ptr2) {
    printf("Pass: Deq head put\n");
  } else {
    printf("Fail: Deq head put\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for Deq tail put
 */
static void testDeqTailPut() {
  Deq q = deq_new();
  int num1 = 1;
  int *ptr1 = &num1;
  deq_tail_put(q, ptr1);
  int num2 = 2;
  int *ptr2 = &num2;
  deq_tail_put(q, ptr2);
  if (deq_len(q) == 2 && deq_tail_get(q) == ptr2) {
    printf("Pass: Deq tail put\n");
  } else {
    printf("Fail: Deq tail put\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for Deq head get
 */
static void testDeqHeadGet() {
  Deq q = deq_new();
  int num1 = 1;
  int *ptr1 = &num1;
  deq_head_put(q, ptr1);
  int num2 = 2;
  int *ptr2 = &num2;
  deq_head_put(q, ptr2);
  if (deq_head_get(q) == ptr2 && deq_len(q) == 1) {
    printf("Pass: Deq head get\n");
  } else {
    printf("Fail: Deq head get\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for Deq tail get
 */
static void testDeqTailGet() {
Deq q = deq_new();
int num1 = 1;
int *ptr1 = &num1;
deq_tail_put(q, ptr1);
int num2 = 2;
int *ptr2 = &num2;
deq_tail_put(q, ptr2);
if (deq_tail_get(q) == ptr2 && deq_len(q) == 1) {
printf("Pass: Deq tail get\n");
} else {
printf("Fail: Deq tail get\n");
}
deq_del(q, 0);
}
/*

This is a test case for Deq head remove
*/
static void testDeqHeadRem() {
Deq q = deq_new();
int num1 = 1;
int *ptr1 = &num1;
deq_head_put(q, ptr1);
int num2 = 2;
int *ptr2 = &num2;
deq_head_put(q, ptr2);
if (deq_head_rem(q,0) == ptr2 && deq_len(q) == 1 && deq_head_get(q) == ptr1) {

printf("Pass: Deq head remove\n");
} else {
printf("Fail: Deq head remove\n");
}
deq_del(q, 0);
}
/*

This is a test case for Deq tail remove
*/
static void testDeqTailRem() {
Deq q = deq_new();
int num1 = 1;
int *ptr1 = &num1;
deq_tail_put(q, ptr1);
int num2 = 2;
int *ptr2 = &num2;
deq_tail_put(q, ptr2);
if (deq_head_rem(q,0) == ptr2 && deq_len(q) == 1 && deq_head_get(q) == ptr1) {

printf("Pass: Deq tail remove\n");
} else {
printf("Fail: Deq tail remove\n");
}
deq_del(q, 0);
}
/*

This is a test case for Deq to string
*/
static void testDeqToString() {
Deq q = deq_new();
int num1 = 1;
int *ptr1 = &num1;
deq_tail_put(q, ptr1);
int num2 = 2;
int *ptr2 = &num2;
deq_tail_put(q, ptr2);
const char *str = deq_str(q,0);
char *expectedStr = "[1, 2]";
if (strcmp(str, expectedStr) == 0) {
printf("Pass: Deq to string\n");
} else {
printf("Fail: Deq to string\n");
}
free(str);
deq_del(q, 0);
}

 
