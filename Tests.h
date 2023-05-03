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
bool Test_Put_AddToHeadEmptyList();
bool Test_Put_AddToHead2Item();
bool Test_Put_AddToHead3Item();

// Put tail tests
bool Test_Put_AddToTailEmptyList();
bool Test_Put_Add2Tail();
bool Test_Put_Add3Tail();

// Put mixed tests
bool Test_Put_AddToTailEmptyList();
bool Test_Put_Add2Head1Tail();
bool Test_Put_Add2Tail1Head();

// Get head tests
bool Test_Get_Head_EmptyList();
bool Test_Get_Head_1Item();
bool Test_Get_AddToHead2ItemGetHead();

// Get tail tests
bool Test_Get_Tail_EmptyList();
bool Test_Get_Tail_1Item();
bool Test_Get_AddToTail2ItemGetTail();

// Get mixed tests
bool Test_Get_Add2Head2TailGetTail();
bool Test_Get_Add2Tail2HeadGetHead();
bool Test_Get_Add3TailGetHead();

// ith head tests
bool Test_ith_Head_EmptyList();
bool Test_ith_Head_OutOfBounds();
bool Test_ith_Head_1Item();
bool Test_ith_Head_3Item();

// ith tail tests
bool Test_ith_Tail_EmptyList();
bool Test_ith_Tail_OutOfBounds();
bool Test_ith_Tail_1Item();
bool Test_ith_Tail_3Item();

// rem head tests
bool Test_rem_Head_EmptyList();
bool Test_rem1_Head_Add1();
bool Test_rem1_Head_Add2();
bool Test_remMiddle_Head_Add3();
bool Test_remHead_AddHead();

// rem tail tests
bool Test_rem_Tail_EmptyList();
bool Test_rem1_Tail_Add1();
bool Test_rem1_Tail_Add2();
bool Test_remMiddle_Tail_Add3();
bool Test_remMiddle_Tail_Add3();
bool Test_remTail_AddTail();

// rem mixed tests
bool Test_remMiddle_GetHeadTail();
bool Test_remMiddle_GetTailHead();
