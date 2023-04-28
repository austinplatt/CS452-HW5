#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "deq.h"

// Enumeration for different types of Test Steps
typedef enum  {PUT, GET, REM, ith}TestType;

// End in which a test step can be executed on
typedef enum {Head, Tail} End;

// Holds information about a test step.
typedef struct TestStep{
    End end;
    TestType type;
    int* data;
}*TestStep;

// Holds information about the entire test
typedef struct Test{
    TestStep* steps;
    int count;
    char *expected;
}*Test;

// Test Framework functions
char* printInt(Data d);
bool checkHead(char, char);
bool checkTail(char, char);
bool compareEntireString(char*, char*);
void freeInt(Data d);
Test GenerateTest(TestType*, End*, int*, char*, int);
bool runTest();
void printReturn(char*, Data);

bool testOne();
bool testTwo();
bool testThree();
bool testFour();
bool testFive();
bool testSix();