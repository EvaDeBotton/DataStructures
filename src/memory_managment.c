/******************************************************************************
* Project: memory managment                                                   *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
******************************************************************************/

#include <stdlib.h> /* free, malloc */


#define FAILUER               1
#define SUCCESS               0 
#define COUNTER_EMPTY         0 
#define NUM_OF_BLOCKS        10

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);

/*******************************************************************************
********************************* The Main ************************************/

int main ()
{  
    const char *tests_name = "Memory Managment Tests";

    BeginingOfTests(tests_name);

    /* The main test functions */ 

    EndOfTests();
    
    return 0; 
}


/*******************************************************************************
******************************* Begining Of Tests *****************************/

static void BeginingOfTests(const char *tests_name)
{
    printf(YELLOW);
    printf("\n______________________ %s _____________________\n\n", tests_name);
}

/*******************************************************************************
********************************** End Of Tests *******************************/

static void EndOfTests(void)
{
    const char *end = "End Of Tests";

    printf(YELLOW);
    printf("\n_________________________ %s _______________________\n\n", end);
}
