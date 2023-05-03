#include "Tests.h"

// <summary>Takes test information and converts it into a list of test steps</summary>
// <param name = "type">List of test types to run</param>
// <param name = "ends">Which end the test should operate on</param>
// <param name = "data">The list of data associated with a test</param>
// <param name = "expected">Expected state of list after test is run</param>
// <param name = "stepCount">Number of test steps</param>
// <returns>A structure containing list of steps</returns>
Test GenerateTest(TestType *type, End *ends, int *data, char *expected, int stepCount)
{
    Test testList = NULL;
    TestStep *currentStep = NULL;
    if (stepCount > 0)
    {
        // Allocates memory for the test
        testList = malloc(sizeof(struct Test));
        currentStep = (TestStep *)malloc(sizeof(TestStep) * stepCount);
    }
    // Goes through each type, end, and data to build test
    for (int i = 0; i < stepCount; i++)
    {
        TestType currentType = *type;
        TestType end = *ends;

        *(currentStep + i) = malloc(sizeof(struct TestStep));

        // Sets test step information from passed in values
        (*(currentStep + i))->type = currentType;
        (*(currentStep + i))->end = end;

        // Iterates the pointers to get the next value
        type += 1;
        ends += 1;

        // Get doesn't need Data so don't increase data or set data value
        if (currentType != GET)
        {
            (*(currentStep + i))->data = data;
            data += 1;
        }
    }
    // Sets testList steps to generated steps
    testList->steps = currentStep;

    // Set other information related to a test
    testList->expected = expected;
    testList->count = stepCount;

    return testList;
}

// <summary>Runs test based off of generated structure</summary>
// <param name = "testList">List of tests and associated test info</param>
// <returns>True if test passed, false otherwise</returns>
bool runTest(Test testList)
{
    int **data = NULL;
    Deq q = deq_new();
    if (testList->count > 0)
    {
        // Initializes array to hold data
        data = malloc(sizeof(int *) * testList->count);
    }
    bool passed = false;

    // Runs through each test step and executes the step
    for (int i = 0; i < testList->count; i++)
    {
        // Gets the current step
        struct TestStep step = *(*((testList->steps) + i));

        // Gets data at the current step
        *(data + i) = (step).data;

        // Determine what test type should be run
        if (step.type == PUT)
        {
            if (step.end == Head)
            {
                deq_head_put(q, *(data + i));
            }
            else
            {
                deq_tail_put(q, *(data + i));
            }
        }
        else if (step.type == GET)
        {
            if (step.end == Head)
            {
                Data d = deq_head_get(q);
                printReturn("Head get", d);
            }
            else
            {
                Data d = deq_tail_get(q);
                printReturn("Tail get", d);
            }
        }
        else if (step.type == ith)
        {
            if (step.end == Head)
            {
                Data d = deq_head_ith(q, *(*(data + i)));
                printReturn("Head ith", d);
            }
            else
            {
                Data d = deq_tail_ith(q, *(*(data + i)));
                printReturn("Tail ith", d);
            }
        }
        else if (step.type == REM)
        {
            // Saves REM step data if data was not found
            int *functionAddress = step.data;

            // Gets value to search for
            int searchVal = *functionAddress;

            // Looks at previous steps to determine if the value exists in list
            for (int j = i - 1; j >= 0; j--)
            {
                struct TestStep currentStep = *(*((testList->steps) + j));

                // Checks if the previous step contains same data as remove step
                if (*currentStep.data == searchVal)
                {
                    // If found then set the address to remove to the currenStep's data address
                    functionAddress = currentStep.data;
                    break;
                }
            }

            if (step.end == Head)
            {
                Data d = deq_head_rem(q, functionAddress);
                printReturn("Head remove", d);
            }
            else
            {
                Data d = deq_tail_rem(q, functionAddress);
                printReturn("Tail remove", d);
            }
        }
    }

    // Evaluates result of test
    Data head = deq_head_ith(q, 0);
    Data tail = deq_tail_ith(q, 0);

    if (head == NULL || tail == NULL)
    {
        if (testList->expected == NULL)
        {
            printf("%s", "PASSED: list was NULL with expected result NULL\n");
        }
        else
        {
            fprintf(stderr, "%s%s", "Failed result was null when expected was", testList->expected);
        }
    }
    else
    {
        char *headString = printInt(head);
        char *tailString = printInt(tail);

        char headChar = headString[0];
        char tailChar = tailString[0];

        char expectedHead = testList->expected[0];
        char expectedTail = testList->expected[strlen(testList->expected) - 1];

        passed = checkHead(headChar, expectedHead);

        bool result = checkTail(tailChar, expectedTail);

        passed = (passed == false ? passed : result);

        char *actual = deq_str(q, printInt);

        result = compareEntireString(actual, testList->expected);
        passed = (passed == false ? passed : result);

        if (passed)
        {
            printf("%s", "PASSED\n");
        }
        else
        {
            printf("%s", "FAILED\n");
        }

        free(actual);
        free(headString);
        free(tailString);
    }
    // Frees all structures generated
    for (int i = 0; i < testList->count; i++)
    {
        free(*((testList->steps) + i));
    }
    free(testList->steps);
    free(testList);
    free(data);
    deq_del(q, 0);

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

// Tests adding a value to the head of an empty list
bool Test_Put_AddToHeadEmptyList()
{

    printf("%s", "\nRunning function : Test_Put_AddToHeadEmptyList\n");
    int stepCount = 1;
    int data[] = {5};
    char *expected = "5";

    TestType types[] = {PUT};
    End end[] = {Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests adding three items to the head of the list
bool Test_Put_AddToHead3Item()
{

    printf("%s", "\nRunning function : Test_Put_AddToHead3Item\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "5 6 7";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests putting 3 items at the tail of an empty list
bool Test_Put_Add3Tail()
{
    printf("%s", "\nRunning function : Test_Put_Add3Tail\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "5 6 7";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
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

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting an item from the head of a list with one item
bool Test_Get_Head_1Item()
{
    printf("%s", "\nRunning function : Test_Get_Head_1Item\n");
    int stepCount = 2;
    int data[] = {5};
    char *expected = NULL;

    TestType types[] = {PUT, GET};
    End end[] = {Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting an item from the head of a list with two items
bool Test_Get_AddToHead2ItemGetHead()
{

    printf("%s", "\nRunning function : Test_Get_AddToHead2ItemGetHead\n");
    int stepCount = 3;
    int data[] = {5, 6};
    char *expected = "5";

    TestType types[] = {PUT, PUT, GET};
    End end[] = {Head, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting an item from the tail of a list with one item
bool Test_Get_Tail_1Item()
{
    printf("%s", "\nRunning function : Test_Get_Tail_1Item\n");
    int stepCount = 2;
    int data[] = {5};
    char *expected = NULL;

    TestType types[] = {PUT, GET};
    End end[] = {Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests Adding two items to the tail and then getting from tail
bool Test_Get_AddToTail2ItemGetTail()
{

    printf("%s", "\nRunning function : Test_Get_AddToTail2ItemGetTail\n");
    int stepCount = 3;
    int data[] = {5, 6};
    char *expected = "5";

    TestType types[] = {PUT, PUT, GET};
    End end[] = {Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests adding two the head, getting from tail and then putting at tail
bool Test_Get_Add2Head2TailGetTail()
{

    printf("%s", "\nRunning function : Test_Get_Add2Head2TailGetTail\n");
    int stepCount = 4;
    int data[] = {5, 6, 7};
    char *expected = "6 7";

    TestType types[] = {PUT, PUT, GET, PUT};
    End end[] = {Head, Head, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests adding two items to tail getting from the head and adding to the head
bool Test_Get_Add2Tail2HeadGetHead()
{
    printf("%s", "\nRunning function : Test_Get_Add2Tail2HeadGetHead\n");
    int stepCount = 4;
    int data[] = {5, 6, 7};
    char *expected = "7 6";

    TestType types[] = {PUT, PUT, GET, PUT};
    End end[] = {Tail, Tail, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests adding two items to the tail getting from the head and then adding to the tail
bool Test_Get_Add3TailGetHead()
{
    printf("%s", "\nRunning function : Test_Get_Add3TailGetHead\n");
    int stepCount = 4;
    int data[] = {5, 6, 7};
    char *expected = "6 7";

    TestType types[] = {PUT, PUT, GET, PUT};
    End end[] = {Tail, Tail, Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting ith from head for empty list
bool Test_ith_Head_EmptyList()
{
    printf("%s", "\nRunning function : Test_ith_Head_EmptyList\n");
    int stepCount = 1;
    int data[] = {0};
    char *expected = NULL;

    TestType types[] = {ith};
    End end[] = {Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting ith from the tail with an empty list
bool Test_ith_Tail_EmptyList()
{
    printf("%s", "\nRunning function : Test_ith_Tail_EmptyList\n");
    int stepCount = 1;
    int data[] = {0};
    char *expected = NULL;

    TestType types[] = {ith};
    End end[] = {Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting an item from the head that is out of bounds
bool Test_ith_Head_OutOfBounds()
{
    printf("%s", "\nRunning function : Test_ith_Head_OutOfBounds\n");
    int stepCount = 2;
    int data[] = {5, 1};
    char *expected = "5";

    TestType types[] = {PUT, ith};
    End end[] = {Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting an item from the tail that is out of bounds
bool Test_ith_Tail_OutOfBounds()
{
    printf("%s", "\nRunning function : Test_ith_Tail_OutOfBounds\n");
    int stepCount = 2;
    int data[] = {5, 1};
    char *expected = "5";

    TestType types[] = {PUT, ith};
    End end[] = {Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting ith from the head with only one item
bool Test_ith_Head_1Item()
{
    printf("%s", "\nRunning function : Test_ith_Head_1Item\n");
    int stepCount = 2;
    int data[] = {5, 0};
    char *expected = "5";

    TestType types[] = {PUT, ith};
    End end[] = {Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting ith from the tail with only one item
bool Test_ith_Tail_1Item()
{
    printf("%s", "\nRunning function : Test_ith_Tail_1Item\n");
    int stepCount = 2;
    int data[] = {5, 0};
    char *expected = "5";

    TestType types[] = {PUT, ith};
    End end[] = {Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting item at the tail starting at head 
bool Test_ith_Head_3Item()
{
    printf("%s", "\nRunning function : Test_ith_Head_3Item\n");
    int stepCount = 4;
    int data[] = {5, 6, 7, 2};
    char *expected = "7 5 6";

    TestType types[] = {PUT, PUT, PUT, ith};
    End end[] = {Head, Tail, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests getting item from head starting at tail
bool Test_ith_Tail_3Item()
{
    printf("%s", "\nRunning function : Test_ith_Tail_3Item\n");
    int stepCount = 4;
    int data[] = {5, 6, 7, 2};
    char *expected = "7 5 6";

    TestType types[] = {PUT, PUT, PUT, ith};
    End end[] = {Head, Tail, Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing from head with an empty list
bool Test_rem_Head_EmptyList()
{
    printf("%s", "\nRunning function : Test_rem_Head_EmptyList\n");
    int stepCount = 1;
    int data[] = {3};
    char *expected = NULL;

    TestType types[] = {REM};
    End end[] = {Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing from tail with an empty list
bool Test_rem_Tail_EmptyList()
{
    printf("%s", "\nRunning function : Test_rem_Tail_EmptyList\n");
    int stepCount = 1;
    int data[] = {3};
    char *expected = NULL;

    TestType types[] = {REM};
    End end[] = {Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item with one item in the list
bool Test_rem1_Head_Add1()
{
    printf("%s", "\nRunning function : Test_rem1_Head_Add1\n");
    int stepCount = 2;
    int data[] = {6, 6};
    char *expected = NULL;

    TestType types[] = {PUT, REM};
    End end[] = {Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item starting search at head with two items in list
bool Test_rem1_Head_Add2()
{
    printf("%s", "\nRunning function : Test_rem1_Tail_Add2\n");
    int stepCount = 3;
    int data[] = {6, 7, 6};
    char *expected = "7";

    TestType types[] = {PUT, PUT, REM};
    End end[] = {Head, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item starting search at tail with one item in list
bool Test_rem1_Tail_Add1()
{
    printf("%s", "\nRunning function : Test_rem1_Tail_Add1\n");
    int stepCount = 2;
    int data[] = {6, 6};
    char *expected = NULL;

    TestType types[] = {PUT, REM};
    End end[] = {Head, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item starting search at tail with two items in list
bool Test_rem1_Tail_Add2()
{
    printf("%s", "\nRunning function : Test_rem1_Tail_Add2\n");
    int stepCount = 3;
    int data[] = {6, 7, 6};
    char *expected = "7";

    TestType types[] = {PUT, PUT, REM};
    End end[] = {Head, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item starting search at tail with three items in list
bool Test_remMiddle_Tail_Add3()
{
    printf("%s", "\nRunning function : Test_remMiddle_Tail_Add3\n");
    int stepCount = 4;
    int data[] = {6, 7, 8, 7};
    char *expected = "6 8";

    TestType types[] = {PUT, PUT, PUT, REM};
    End end[] = {Head, Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item starting search at head with three items in list
bool Test_remMiddle_Head_Add3()
{
    printf("%s", "\nRunning function : Test_remMiddle_Head_Add3\n");
    int stepCount = 4;
    int data[] = {6, 7, 8, 7};
    char *expected = "6 8";

    TestType types[] = {PUT, PUT, PUT, REM};
    End end[] = {Head, Tail, Tail, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}
// Tests removing item at the tail at then adding an item at the tail
bool Test_remTail_AddTail()
{
    printf("%s", "\nRunning function : Test_remTail_AddTail\n");
    int stepCount = 5;
    int data[] = {6, 7, 8, 8, 8};
    char *expected = "6 7 8";

    TestType types[] = {PUT, PUT, PUT, REM, PUT};
    End end[] = {Head, Tail, Tail, Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing item from the head and then adding back at the head
bool Test_remHead_AddHead()
{
    printf("%s", "\nRunning function : Test_remHead_AddHead\n");
    int stepCount = 5;
    int data[] = {6, 7, 8, 6, 9};
    char *expected = "9 7 8";

    TestType types[] = {PUT, PUT, PUT, REM, PUT};
    End end[] = {Head, Tail, Tail, Tail, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing an item from the middle of a list with 4 elements and ensures pointers to the new middle object are accurate
bool Test_remMiddle_GetHeadTail()
{
    printf("%s", "\nRunning function : Test_remMiddle_GetHeadTail\n");
    int stepCount = 5;
    int data[] = {6, 7, 8, 7, 1};
    char *expected = "6 8";

    TestType types[] = {PUT, PUT, PUT, REM, ith};
    End end[] = {Head, Tail, Tail, Tail, Tail};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

// Tests removing an item from the middle of a list with 4 elements and ensures pointers to the new middle object are accurate
bool Test_remMiddle_GetTailHead()
{
    printf("%s", "\nRunning function : Test_remMiddle_GetTailHead\n");
    int stepCount = 5;
    int data[] = {6, 7, 8, 7, 1};
    char *expected = "6 8";

    TestType types[] = {PUT, PUT, PUT, REM, ith};
    End end[] = {Head, Tail, Tail, Head, Head};

    Test test = GenerateTest(types, end, data, expected, stepCount);
    return runTest(test);
}

 
