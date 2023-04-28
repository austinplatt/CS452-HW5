#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"

int main() {

testOne();
  testTwo();
  testThree();
  testFour();
  testFive();
  testSix();
}

/*
 * This is a test case for adding at the tail of an empty list
 */

static void testOne()
{
  Deq q = deq_new();
  char letter1 = 'z';
  char *point = &letter1;
  deq_head_put(q, point); 
  char letter2 = 'y';
  char *point2 = &letter2;
  deq_head_put(q, point2);
  if (point == deq_tail_get(q)) 
  {
    printf("Pass: Test 1\n");
  } else 
  {
    printf("Fail: Test 1\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for adding at the head of an empty list
 */
static void testTwo()
{
  Deq q = deq_new();
  char letter1 = 'z';
  char *point = &letter1;
  deq_head_put(q, point);
  if (deq_len(q) == 1) 
  {
    printf("Pass: Test 2\n");
  } else 
  {
    printf("Fail: Test 2\n");
  }
  deq_del(q, 0);
}

/*
 * This is a test case for removing at the tail of an empty
 * list. 
 */
static void testThree()
{
  Deq q=deq_new();
  if(deq_tail_get(q)==NULL)
  {
    printf("Pass: Test 3\n");
  }
  else
  {
    printf("Fail: Test 3\n");
  }
  deq_del(q,0);
}

/*
 * This is a test case for removing at the head of an empty
 * list.
 */
static void testFour()
{
  Deq q=deq_new();
  if(deq_head_get(q)==NULL)
  {
    printf("Pass: Test 4\n");
  }
  else
  {
    printf("Fail: Test 4\n");
  }
  deq_del(q,0);
}

/*
 * This is a test case for removing a specific data at the tail 
 * of an intended empty list. 
 * list.
 */

static void testFive()
{
  Deq q=deq_new();
  if(deq_head_rem(q,0)==NULL)
  {
    printf("Pass: Test 5\n");
  }
  else
  {
    printf("Fail: Test 5\n");
  }
  deq_del(q,0);
}

/*
 * This is a test case for removing a specific data at the head 
 * of an intended empty list. 
 * list.
 */

static void testSix() 
{
  Deq q=deq_new();
  if(deq_head_rem(q,0)==NULL)
  {
    printf("Pass: Test 6\n");
  }
  else
  {
    printf("Fail: Test 6\n");
  }
  deq_del(q,0);
}
