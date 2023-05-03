#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "deq.h"

//The enumeration of various Test Step types
typedef enum  {PUT, GET, REM, ith}TestType;

//The endpoints on which a test step may be performed
typedef enum {Head, Tail} End;

//Holds data regarding a test phase.
typedef struct TestStep{
    End end;
    TestType type;
    int* data;
}*TestStep;

//Contains data regarding the whole test.
typedef struct Test{
    TestStep* steps;
    int count;
    char *expected;
}*Test;

//Testing Framework performs
char* printInt(Data d);
bool checkHead(char, char);
bool checkTail(char, char);
bool compareEntireString(char*, char*);
void freeInt(Data d);
Test GenerateTest(TestType*, End*, int*, char*, int);
bool runTest();
void printReturn(char*, Data);

//The put head tests
bool Test_Put_AddToHeadEmptyList(); 
bool Test_Put_AddToHead2Item(); 

//The put tail tests
bool Test_Put_AddToTailEmptyList(); 
bool Test_Put_Add2Tail(); 

//The tail/head put tests
bool Test_Put_AddToTailEmptyList(); 
bool Test_Put_Add2Tail1Head(); 

//The get head test
bool Test_Get_Head_EmptyList(); 

//The get tail test
bool Test_Get_Tail_EmptyList(); 

