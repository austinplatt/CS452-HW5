#include "Tests.h"

// <summary>Takes test information and converts it into a list of test steps</summary>
// <param name = "type">List of test types to run</param>
// <param name = "ends">Which end the test should operate on</param>
// <param name = "data">The list of data associated with a test</param>
// <param name = "expected">Expected state of list after test is run</param>
// <param name = "stepCount">Number of test steps</param>
// <returns>A structure containing list of steps</returns>
Test MakeTest(TestType *typeList, End *endList, int *dataList, char *expectedResult, int numSteps)
{
    Test testList = NULL;
    TestStep *stepList = NULL;
    if (numSteps > 0)
    {
        // Allocates memory for the test
        testList = malloc(sizeof(struct Test));
        stepList = (TestStep *)malloc(sizeof(TestStep) * numSteps);
    }
    // Goes through each type, end, and data to build test
    for (int i = 0; i < numSteps; i++)
    {
        TestType currentType = *typeList;
        TestType end = *endList;

        *(stepList + i) = malloc(sizeof(struct TestStep));

        // Sets test step information from passed in values
        (*(stepList + i))->type = currentType;
        (*(stepList + i))->end = end;

        // Iterates the pointers to get the next value
        typeList += 1;
        endList += 1;

        // Get doesn't need Data so don't increase data or set data value
        if (currentType != GET)
        {
            (*(stepList + i))->data = dataList;
            dataList += 1;
        }
    }
    // Sets testList steps to generated steps
    testList->steps = stepList;

    // Set other information related to a test
    testList->expected = expectedResult;
    testList->count = numSteps;

    return testList;
}

// <summary>Runs test based off of generated structure</summary>
// <param name = "testList">List of tests and associated test info</param>
// <returns>True if test passed, false otherwise</returns>
bool executeTest(Test testList) {
    Deq q = deq_new();
    bool passed = false;

    for (int i = 0; i < testList->count; i++) {
        struct TestStep* step = *((testList->steps) + i);

        switch (step->type) {
            case PUT:
                if (step->end == Head) {
                    deq_head_put(q, step->data);
                } else {
                    deq_tail_put(q, step->data);
                }
                break;
            case GET:
                if (step->end == Head) {
                    Data d = deq_head_get(q);
                    printReturn("Head get", d);
                } else {
                    Data d = deq_tail_get(q);
                    printReturn("Tail get", d);
                }
                break;
            case ith:
                if (step->end == Head) {
                    Data d = deq_head_ith(q, *(step->data));
                    printReturn("Head ith", d);
                } else {
                    Data d = deq_tail_ith(q, *(step->data));
                    printReturn("Tail ith", d);
                }
                break;
            case REM:
                int* functionAddress = step->data;
                int searchVal = *functionAddress;

                for (int j = i - 1; j >= 0; j--) {
                    struct TestStep* currentStep = *((testList->steps) + j);

                    switch (currentStep->type) {
                        case PUT:
                        case GET:
                        case ith:
                            if (*(currentStep->data) == searchVal) {
                                functionAddress = currentStep->data;
                                break;
                            }
                            break;
                    }
                }

                if (step->end == Head) {
                    Data d = deq_head_rem(q, functionAddress);
                    printReturn("Head remove", d);
                } else {
                    Data d = deq_tail_rem(q, functionAddress);
                    printReturn("Tail remove", d);
                }
                break;
            default:
                // ignore unknown step types
                break;
        }
    }

    Data head = deq_head_ith(q, 0);
    Data tail = deq_tail_ith(q, 0);

    if (head == NULL || tail == NULL) {
        if (testList->expected == NULL) {
            printf("PASSED: list was NULL with expected result NULL\n");
            passed = true;
        } else {
            fprintf(stderr, "Failed result was null when expected was %s", testList->expected);
        }
    } else {
        char* actual = deq_str(q, printInt);
        passed = compareEntireString(actual, testList->expected);
        if (passed) {
            printf("PASSED\n");
        } else {
            printf("FAILED\n");
        }
        free(actual);
    }

    deq_del(q, 0);

    // Free memory
    for (int i = 0; i < testList->count; i++) {
        struct TestStep* step = *((testList->steps) + i);
        free(step->data);
        free(step);
    }
    free(testList->steps);
    free(testList);

    return passed;
}

// <summary>Gets string representation of integer</summary>
// <param name = "d">Data to convert to string</param>
// <returns>The string representation of an integer</returns>
char *printInt(Data d)
{
    if (d != NULL)
    {
        int value = *((int *)(d));
        int size = sizeof(char) * sizeof(int) * 4 + 1;
        char *string = malloc(sizeof(size));

        sprintf(string, "%d", value);

        return string;
    }
    else
    {
        char *string = "";
        return string;
    }
}

// <summary>Compares expected and actual result to determine if structure is equal</summary>
// <param name = "actual">Output of data structure</param>
// <param name = "expected">What the output of the data structure should be</param>
// <returns>True if string is equal, false otherwise</returns>
bool compareEntireString(char *actual, char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        printf("%s", "String comparison passed\n");
        return true;
    }
    fprintf(stderr, "%s%s%s%s%s", "String comparison failed\n Actual:", actual, "\nExpected:", expected, "\n");
    return false;
}

// <summary>Determines if the head is equal to the expected result</summary>
// <param name = "actual">What the actual head is</param>
// <param name = "expected">What the head should be</param>
// <returns>True if actual and expected are the same false otherwise</returns>
bool checkHead(char actual, char expected)
{

    if (actual == expected)
    {
        printf("%s", "Head check passed\n");
        return true;
    }
    fprintf(stderr, "%s%c%s%c%s", "Actual head ", actual, "is not equal to expected", expected, "\n");
    return false;
}

// <summary>Prints the return value of the function that was ran</summary>
// <param name = "function">Type of function ran</param>
// <param name = "d">The data returned by the function</param>
void printReturn(char *function, Data d)
{
    if (d == NULL)
    {
        printf("%s%s", function, " Return data was null\n");
    }
    else
    {
        char *returnVal = printInt(d);
        printf("%s%s%s%s", function, " Return value was: ", returnVal, "\n");
        free(returnVal);
    }
}

// <summary>Checks if the tail is equal to the expected tail</summary>
// <param name = "actual">The actual tail value</param>
// <param name = "expected">The expected tail value</param>
// <returns>True if tail is equal to expected, false otherwise</returns>
bool checkTail(char actual, char expected)
{
    if (actual == expected)
    {
        printf("%s", "Tail check passed\n");
        return true;
    }
    fprintf(stderr, "%s%c%s%c", "Actual tail ", actual, "is not equal to expected\n", expected);
    return false;
}

// Tests getting an item from head of an empty list
bool Test_Get_Head_EmptyList()
{
    printf("%s", "\nRunning function : Test_Get_Head_EmptyList\n");
    int stepCount = 1;
    int data[] = {};
    char *expected = NULL;

    TestType types[] = {GET};
    End end[] = {Head};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests getting an item from the tail of an empty list
bool Test_Get_Tail_EmptyList()
{
    printf("%s", "\nRunning function : Test_Get_Tail_EmptyList\n");
    int stepCount = 1;
    int data[] = {};
    char *expected = NULL;

    TestType types[] = {GET};
    End end[] = {Tail};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests adding a value to the head of an empty list
bool Test_Put_AddToHeadEmptyList()
{

    printf("%s", "\nRunning function : Test_Put_AddToHeadEmptyList\n");
    int stepCount = 1;
    int data[] = {5};
    char *expected = "5";

    TestType types[] = {PUT};
    End end[] = {Head};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests adding an item to the tail of an empty list
bool Test_Put_AddToTailEmptyList()
{
    printf("%s", "\nRunning function : Test_Put_AddToTailEmptyList\n");
    int stepCount = 1;
    int data[] = {5};
    char *expected = "5";

    TestType types[] = {PUT};
    End end[] = {Tail};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests putting two items at the head of the list
bool Test_Put_AddToHead2Item()
{

    printf("%s", "\nRunning function : Test_Put_AddToHead2Item\n");
    int stepCount = 2;
    int data[] = {5, 6};
    char *expected = "6 5";

    TestType types[] = {PUT, PUT};
    End end[] = {Head, Head};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests putting two items at the tail of the empty list
bool Test_Put_Add2Tail()
{
    printf("%s", "\nRunning function : Test_Put_Add2Tail\n");
    int stepCount = 2;
    int data[] = {5, 7};
    char *expected = "5 7";

    TestType types[] = {PUT, PUT};
    End end[] = {Tail, Tail};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}


// Tests adding two items to the tail and then adding an item to the head
bool Test_Put_Add2Tail1Head()
{

    printf("%s", "\nRunning function : Test_Put_Add2Tail1Head\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "7 5 6";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Tail, Tail, Head};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

// Tests adding two items to the head and then 1 to the tail
bool Test_Put_Add2Head1Tail()
{

    printf("%s", "\nRunning function : Test_Put_Add2Head1Tail\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "6 5 7";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Head, Head, Tail};

    Test test = makeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}




 
