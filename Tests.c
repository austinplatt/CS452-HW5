#include "Tests.h"
/**
 * This function creates a new test with a list of steps and returns a pointer to it. The test type, 
 * the end point, the data, the expected result, and the number of steps are passed as arguments.
 */ 
Test MakeTest(TestType *typeList, End *endList, int *dataList, char *expectedResult, int numSteps)
{
    Test testList = NULL;
    TestStep *stepList = NULL;
    
    //Allocates memory for the test and step lists if there are steps to be executed.
    if (numSteps > 0)
    {
        testList = malloc(sizeof(struct Test));
        stepList = (TestStep *)malloc(sizeof(TestStep) * numSteps);
    }
    //Loops through the steps and assign their type, endpoint, and data.
    for (int i = 0; i < numSteps; i++)
    {
        TestType currentType = *typeList;
        TestType end = *endList;

        //Allocates memory for the current step.
        *(stepList + i) = malloc(sizeof(struct TestStep));

        // Assign the current step's type, endpoint, and data.
        (*(stepList + i))->type = currentType;
        (*(stepList + i))->end = end;

        typeList += 1;
        endList += 1;

        //If the current step is not a GET operation, assign its data.
        if (currentType != GET)
        {
            (*(stepList + i))->data = dataList;
            dataList += 1;
        }
    }
    //Assign the test's step list, expected result, and number of steps.
    testList->steps = stepList;
    testList->expected = expectedResult;
    testList->count = numSteps;

    // Return the new test.
    return testList;
}
/**
 * This function executes a given test and returns whether the test passed or not.
 */
bool executeTest(Test testList) {
    //Creates a new deque for the test.
    Deq q = deq_new();
    // Allocate an array to store the data values for each step in the test.
    int *data[testList->count];
    //Initializes the passed variable as false.
    bool passed = false;

    //Loops through each step in the test and execute it.
    for (int i = 0; i < testList->count; i++) {
        //Gets the current step and its data.
        struct TestStep *step = *(testList->steps + i);
        int *stepData = step->data;

        // Store the step's data value in the data array.
        data[i] = stepData;

        // Execute the current step based on its type and endpoint.
        if (step->type == PUT) { //if the current step is a PUT operation
            if (step->end == Head) {
                deq_head_put(q, stepData); //put the data at the head of the queue
            } else {
                deq_tail_put(q, stepData); //put the data at the tail of the queue
            }
        } else if (step->type == GET) { // if the current step is a GET operation
            Data d;
            if (step->end == Head) {
                d = deq_head_get(q); //get the data from the head of the queue
                printReturn("Head get", d);
            } else {
                d = deq_tail_get(q); // get the data from the tail of the queue
                printReturn("Tail get", d);
            }
            if (d == NULL) {
                return false;
            }
        } else if (step->type == ith) { //if the current step is an ith operation
            Data d;
            if (step->end == Head) {
                d = deq_head_ith(q, *stepData);
                printReturn("Head ith", d); //get the ith element from the head of the queue
            } else {
                d = deq_tail_ith(q, *stepData);
                printReturn("Tail ith", d); //get the ith element from the tail of the queue
            }
            if (d == NULL) {
                return false;
            }
        } else if (step->type == REM) { // if the current step is a REM operation
            int searchVal = *stepData;
            int *functionAddress = NULL;

            // search for the last step with data equal to searchVal
            for (int j = i - 1; j >= 0; j--) {
                struct TestStep *prevStep = *(testList->steps + j);

                if (*prevStep->data == searchVal) {
                    functionAddress = prevStep->data; //set the function address to the data of the matching step
                    break;
                }
            }
            //If the function address is not found, return false.
            if (functionAddress == NULL) {
                return false;
            }

            Data d;
            //If the current step is a "put" at the head, call deq_head_rem to remove 
            //the element from the head of the deque.
            if (step->end == Head) {
                d = deq_head_rem(q, functionAddress);
                printReturn("Head remove", d);
            } else {
                //Otherwise, if the current step is a "put" at the tail, call deq_tail_rem to remove 
                //the element from the tail of the deque.
                d = deq_tail_rem(q, functionAddress);
                printReturn("Tail remove", d);
            }
            if (d == NULL) {
                return false;
            }
        }
    }

    //Get the first element of the deque using deq_head_ith and the last element using deq_tail_ith.
    Data head = deq_head_ith(q, 0);
    Data tail = deq_tail_ith(q, 0);

    //Check if either head or tail is NULL.
    if (head == NULL || tail == NULL) {
        //If both head and tail are NULL and the expected result is also NULL, print PASSED.
        if (testList->expected == NULL) {
            printf("%s", "PASSED: list was NULL with expected result NULL\n");
        } else {
            //Print the expected result that was provided.
            fprintf(stderr, "%s%s", "Failed result was null when expected was", testList->expected);
        }
    } else {
        //If both head and tail are not NULL, print the first character of head and tail 
        //and save them in headChar and tailChar,
        char *headString = printInt(head);
        char *tailString = printInt(tail);

        char headChar = headString[0];
        char tailChar = tailString[0];

        //Get the first and last characters of the expected result and save them in expectedHead and expectedTail
        char expectedHead = testList->expected[0];
        char expectedTail = testList->expected[strlen(testList->expected) - 1];

        //Check if headChar matches expectedHead using checkHead and save the result in passed.
        passed = checkHead(headChar, expectedHead);
        passed = checkTail(tailChar, expectedTail) && passed;

        //Get the entire deque as a string using deq_str with printInt and save it in actual.
        char *actual = deq_str(q, printInt);
        passed = compareEntireString(actual, testList->expected) && passed;

        //If passed is true, print PASSED. Otherwise, print FAILED.
        if (passed) {
            printf("%s", "PASSED\n");
        } else {
            printf("FAILED\n");
        }
        free(actual);
    }
    //Delete the deque using deq_del.
    deq_del(q, 0);

    //Free the memory used by each TestStep and the steps array, and finally free the memory used by testList.
    for (int i = 0; i < testList->count; i++) {
        struct TestStep* step = *((testList->steps) + i);
        free(step->data);
        free(step);
    }
    free(testList->steps);
    free(testList);

    return passed;
}
/**
 * This function takes a pointer to a Data struct, which is casted to an int pointer to extract the value 
 * of the integer. It then converts the integer to a string and returns it.
 */
char *printInt(Data d)
{
    if (d != NULL)
    {
        //Extracts the integer value from the Data pointer
        int value = *((int *)(d));
        //Calculates the size required to store the integer as a string
        int size = sizeof(char) * sizeof(int) * 4 + 1;
        //Allocates memory for the string
        char *string = malloc(sizeof(size));

        //Converts the integer to a string
        sprintf(string, "%d", value);

        return string;
    }
    else
    {
        char *string = "";
        return string;
    }
}

/**
 * This function compares two strings and returns true if they are equal.
 * If the strings are not equal, it prints an error message to stderr.
 *
 */ 
bool compareEntireString(char *actual, char *expected)
{
    //Use strcmp to compare the strings
    if (strcmp(actual, expected) == 0)
    {
        //Prints a success message
        printf("%s", "String comparison passed\n");
        return true;
    }
    //Prints an error message 
    fprintf(stderr, "%s%s%s%s%s", "String comparison failed\n Actual:", actual, "\nExpected:", expected, "\n");
    return false;
}
/**
 * This function checks if the actual character matches the expected character and 
 * returns true if they match. If they do not match, it prints an error message.
 */ 
bool checkHead(char actual, char expected)
{
    //Compares the actual and expected characters
    if (actual == expected)
    {
        //Print a success message 
        printf("%s", "Head check passed\n");
        return true;
    }
    //Prints an error message
    fprintf(stderr, "%s%c%s%c%s", "Actual head ", actual, "is not equal to expected", expected, "\n");
    return false;
}
/**
 * This function takes a function name and a Data pointer and prints the return value of the function.
 * If the Data pointer is null, it prints an error message instead.
 */ 
void printReturn(char *function, Data d)
{
    //Checks if the Data pointer is null
    if (d == NULL)
    {
        //Print an error message
        printf("%s%s", function, " Return data was null\n");
    }
    else
    {
        //Calls printInt to get the return value as a string
        char *returnVal = printInt(d);
        printf("%s%s%s%s", function, " Return value was: ", returnVal, "\n");
        //Free the memory allocated for the string
        free(returnVal);
    }
}
/**
 * This function checks if the actual character at the end of a string matches the expected character and returns 
 * true if they match. If they do not match, it prints an error message 
 */ 
bool checkTail(char actual, char expected)
{
    //Compares the actual and expected characters
    if (actual == expected)
    {
        //Prints a success message
        printf("%s", "Tail check passed\n");
        return true;
    }
    //Prints an error message
    fprintf(stderr, "%s%c%s%c", "Actual tail ", actual, "is not equal to expected\n", expected);
    return false;
}

//Test that gets an item from head of an empty list
bool Test_Get_Head_EmptyList()
{
    printf("%s", "\nRunning function : Test_Get_Head_EmptyList\n");
    int stepCount = 1;
    int data[] = {};
    char *expected = NULL;

    TestType types[] = {GET};
    End end[] = {Head};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that gets an item from the tail of an empty list
bool Test_Get_Tail_EmptyList()
{
    printf("%s", "\nRunning function : Test_Get_Tail_EmptyList\n");
    int stepCount = 1;
    int data[] = {};
    char *expected = NULL;

    TestType types[] = {GET};
    End end[] = {Tail};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that adds an item to the head of an empty list
bool Test_Put_AddToHeadEmptyList()
{

    printf("%s", "\nRunning function : Test_Put_AddToHeadEmptyList\n");
    int stepCount = 1;
    int data[] = {5};
    char *expected = "5";

    TestType types[] = {PUT};
    End end[] = {Head};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that adds an item to the tail of an empty list
bool Test_Put_AddToTailEmptyList()
{
    printf("%s", "\nRunning function : Test_Put_AddToTailEmptyList\n");
    int stepCount = 1;
    int data[] = {5};
    char *expected = "5";

    TestType types[] = {PUT};
    End end[] = {Tail};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that puts two items at the head of the list
bool Test_Put_AddToHead2Item()
{

    printf("%s", "\nRunning function : Test_Put_AddToHead2Item\n");
    int stepCount = 2;
    int data[] = {5, 6};
    char *expected = "6 5";

    TestType types[] = {PUT, PUT};
    End end[] = {Head, Head};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that puts two items at the tail of an empty list
bool Test_Put_Add2Tail()
{
    printf("%s", "\nRunning function : Test_Put_Add2Tail\n");
    int stepCount = 2;
    int data[] = {5, 7};
    char *expected = "5 7";

    TestType types[] = {PUT, PUT};
    End end[] = {Tail, Tail};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}


//Test that adds two items to the tail and adds one item to the head
bool Test_Put_Add2Tail1Head()
{

    printf("%s", "\nRunning function : Test_Put_Add2Tail1Head\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "7 5 6";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Tail, Tail, Head};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}

//Test that adds two items to the head and adds one to the tail
bool Test_Put_Add2Head1Tail()
{

    printf("%s", "\nRunning function : Test_Put_Add2Head1Tail\n");
    int stepCount = 3;
    int data[] = {5, 6, 7};
    char *expected = "6 5 7";

    TestType types[] = {PUT, PUT, PUT};
    End end[] = {Head, Head, Tail};

    Test test = MakeTest(types, end, data, expected, stepCount);
    return executeTest(test);
}




 
