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

// Put head tests
bool Test_Put_AddToHeadEmptyList(); //
bool Test_Put_AddToHead2Item(); //

// Put tail tests
bool Test_Put_AddToTailEmptyList(); //
bool Test_Put_Add2Tail(); //

// Put mixed tests
bool Test_Put_AddToTailEmptyList(); //
bool Test_Put_Add2Tail1Head(); //

// Get head tests
bool Test_Get_Head_EmptyList(); //

// Get tail tests
bool Test_Get_Tail_EmptyList(); //

