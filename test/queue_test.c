/******************************************************************************
* Sorted pqueue_test.c                                                          *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                               *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include "dlist.h"
#include "sorted_list.h"
#include "utils.h"

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10
#define SRC_SIZE              7 
#define DEST_SIZE             3

typedef struct results
{  
	int is_empty;
	size_t size;		
    int next;
    int prev;
	
} results_ty;

struct pqueue
{
    sorted_list_ty *list;    

} pqueue;

/* test functions */
void TestInitializePQueue(void);
void TestFullPQueue(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/* helper */
static void PQueueMultiInsert(pqueue_ty *pqueue, size_t num_of_insert, 
                                                                    int *array);
/* action & match functions */
static int Cmp(const void *data, const void *param);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Priority Queue Tests";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestInitializePQueue();
	TestFullPQueue();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
************************* Test Initialize Double List *************************/

/*	test case: checking all the list parameter after initializing
*/
void TestInitializePQueue()
{ 
	const char *test_name = "INITIALIZE";

	sorted_list_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.data_from_peek = SUCSSES;
	expected_results.data_from_Dequeue = SUCSSES;

	results.is_empty = PQIsEmpty(pqueue);
	results.size = PQSize(pqueue);
	results.data_from_peek = SUCSSES;
	results.data_from_Dequeue = SUCSSES;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	PQDestroy(pqueue);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestFullPQueue(void)
{ 

	const char *test_name = "FULL";

	sorted_list_ty *pqueue= PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};
	int array_of_expected[NUM_OF_ELEMENTS] = {1,2,3,4,5,6,7,8,9,10};

	PQueueMultiInsert(pqueue, NUM_OF_ELEMENTS, array_of_data);

	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		expected_results.is_empty = NOT_EMPTY;
		expected_results.size = NUM_OF_ELEMENTS - i ;
		expected_results.data_from_peek = array_of_expected[NUM_OF_ELEMENTS -i];
		expected_results.data_from_Dequeue = array_of_expected[NUM_OF_ELEMENTS -i];

		results.is_empty = PQIsEmpty(pqueue);
		results.size = PQSize(pqueue);
		results.data_from_peek = PQPeek(pqueue);
		results.data_from_Dequeue = PQDequeue(pqueue); 		                  

		TestResultComper(results, expected_results, test_name, __LINE__);
	}

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.data_from_peek = SUCSSES;
	expected_results.data_from_Dequeue = SUCSSES;

	results.is_empty = PQIsEmpty(pqueue);
	results.size = PQSize(pqueue);
	results.data_from_peek = SUCSSES;
	results.data_from_Dequeue = SUCSSES;		                  

	TestResultComper(results, expected_results, test_name, __LINE__);

	PQDestroy(pqueue);
}

/*******************************************************************************
************************************** Cmp ************************************/

static int Cmp(const void *data, const void *param)
{
	assert(param);
	assert(data);

	return (*(int *)data - *(int *)param);
}

/*******************************************************************************
****************************** dlist Multi Insert *****************************/

static void PQueueMultiInsert(pqueue_ty *pqueue, size_t num_of_insert, 
                                                                     int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		PQEnqueue(pqueue_ty *pqueue, (void *)(array + i))
	}
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "pqueue test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.is_empty != expected_results.is_empty)
	{
		printf(RED);
		printf("\nThe is_empty test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.size != expected_results.size)
	{
		printf(RED);
		printf("\nThe init counter test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.next != expected_results.next)
	{
		printf(RED);
		printf("\nThe Sequence of Next's test fail for %s %s at line %lu\n", 
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.prev != expected_results.prev)
	{
		printf(RED);
		printf("\nThe Sequence of Prev's test fail for %s %s at line %lu\n",
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	} 

	if(test_is_successful_flag == ILRD_TRUE)
	{
		printf(GREEN);
		printf("\n %s %s passed !\n", test_name ,test_object);
	} 
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

/*******************************************************************************
************************************ Remainder ********************************/

static void Remainder(void)
{
	printf(RED);
	puts("\n DESTROY TEST - vlgrain is required \n");
}








