/******************************************************************************
* Sorted pqueue_test.c                                                        *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                                 *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include "dlist.h"
#include <assert.h> /* assert */
#include "dlist.h"
#include "sorted_list.h"
#include "pqueue.h"
#include "utils.h"

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10

typedef struct results
{  
	int is_empty;
	size_t size;		
    void *data_from_peek;
    void *data_from_Dequeue;
	
} results_ty;

struct pqueue
{
    sorted_list_ty *list;    

} pqueue;

/* test functions */
void TestInitializePQueue(void);
void TestPQEnqueue(void);
void TestPQDqueue(void);
void TestPQClear(void);
void TestPQErease(void);
void TestPQEmpty(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static int TestResultComperMulti(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void TestPrintResultComperMulti(int test_is_successful_flag, 
	                                                     const char *test_name);

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
	TestPQEnqueue();
	TestPQDqueue();
	TestPQEmpty();
	TestPQClear();
	TestPQErease();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
*********************** Test Initialize Priority Queue ***********************/

/*	test case: checking all the list parameter after initializing
*/
void TestInitializePQueue()
{ 
	const char *test_name = "INITIALIZE";
	int null = 0; 
	void *ptr_to_null = &null;

	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.data_from_peek = ptr_to_null;
	expected_results.data_from_Dequeue = ptr_to_null;

	results.is_empty = PQIsEmpty(pqueue);
	results.size = PQSize(pqueue);
	results.data_from_peek = ptr_to_null;
	results.data_from_Dequeue = ptr_to_null;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	PQDestroy(pqueue);
}

/*******************************************************************************
********************************* Test PQEnqueue ******************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestPQEnqueue(void)
{ 

	const char *test_name = "ENQUEUE";
	int test_flag = ILRD_TRUE;
	int null = 0; 
	void *ptr_to_null = &null;

	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};
	int array_of_expected[NUM_OF_ELEMENTS] = {6,6,6,6,6,1,1,1,1,1};
	int i = 0;

	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		PQEnqueue(pqueue, (void *)(array_of_data + i));

		expected_results.is_empty = NOT_EMPTY;
		expected_results.size = i + 1;
		expected_results.data_from_peek = (void *)&array_of_expected[i];
		expected_results.data_from_Dequeue = ptr_to_null;

		results.is_empty = PQIsEmpty(pqueue);
		results.size = PQSize(pqueue);
		results.data_from_peek = PQPeek(pqueue);
		results.data_from_Dequeue = ptr_to_null;

		test_flag += TestResultComperMulti(results, expected_results, test_name, __LINE__);		                  
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	PQDestroy(pqueue);
}

/*******************************************************************************
******************************** Test PQDqueue ********************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestPQDqueue(void)
{ 

	const char *test_name = "DEQUEUE";
	int test_flag = ILRD_TRUE;
	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};
	int array_of_expected[NUM_OF_ELEMENTS] = {10,9,8,7,6,5,4,3,2,1};
	int i = 0;

	PQueueMultiInsert(pqueue, NUM_OF_ELEMENTS, array_of_data);

	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		expected_results.is_empty = NOT_EMPTY;
		expected_results.size = NUM_OF_ELEMENTS - i ;
		expected_results.data_from_peek = (void *)&array_of_expected[NUM_OF_ELEMENTS - i - 1];
		expected_results.data_from_Dequeue = (void *)&array_of_expected[NUM_OF_ELEMENTS - i - 1];

		results.is_empty = PQIsEmpty(pqueue);
		results.size = PQSize(pqueue);
		results.data_from_peek = PQPeek(pqueue);
		results.data_from_Dequeue = PQDequeue(pqueue); 

		test_flag += TestResultComperMulti(results, expected_results, test_name, __LINE__);			                  
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	PQDestroy(pqueue);
}

/*******************************************************************************
******************************** Test PQ Empty ********************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestPQEmpty(void)
{ 

	const char *test_name = "EMPTY";
	int null = 0; 
	void *ptr_to_null = &null;

	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};
	int i = 0;

	PQueueMultiInsert(pqueue, NUM_OF_ELEMENTS, array_of_data);

	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		results.data_from_Dequeue = PQDequeue(pqueue); 		                  
	}

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.data_from_peek = ptr_to_null;
	expected_results.data_from_Dequeue = ptr_to_null;

	results.is_empty = PQIsEmpty(pqueue);
	results.size = PQSize(pqueue);
	results.data_from_peek = ptr_to_null;
	results.data_from_Dequeue = ptr_to_null;		                  

	TestResultComper(results, expected_results, test_name, __LINE__);

	PQDestroy(pqueue);
}

/*******************************************************************************
******************************** Test PQ Clear ********************************/

/*	test case: checking all the list parameter after inserting n elements and 
               clear all of them.
*/

void TestPQClear(void)
{ 

	const char *test_name = "CLEAR";
	int null = 0; 
	void *ptr_to_null = &null;

	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};

	PQueueMultiInsert(pqueue, NUM_OF_ELEMENTS, array_of_data);

	PQClear(pqueue);

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.data_from_peek = ptr_to_null;
	expected_results.data_from_Dequeue = ptr_to_null;

	results.is_empty = PQIsEmpty(pqueue);
	results.size = PQSize(pqueue);
	results.data_from_peek = ptr_to_null;
	results.data_from_Dequeue = ptr_to_null;		                  

	TestResultComper(results, expected_results, test_name, __LINE__);

	PQDestroy(pqueue);
}

/*******************************************************************************
******************************** Test PQErease ********************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestPQErease(void)
{ 

	const char *test_name = "EREASE";
	int test_flag = ILRD_TRUE;
	pqueue_ty *pqueue = PQCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {6,7,8,9,10,1,2,3,4,5};
	int array_of_expected[NUM_OF_ELEMENTS] = {4,3,3,3,3,3,3,3,2,1};
	int array_of_erease[NUM_OF_ELEMENTS] = {4,3,8,7,6,5,10,9,2,1};
	int i = 0;



	PQueueMultiInsert(pqueue, NUM_OF_ELEMENTS, array_of_data);

	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		expected_results.is_empty = NOT_EMPTY;
		expected_results.size = NUM_OF_ELEMENTS - i ;
		expected_results.data_from_peek = (void *)&array_of_expected[NUM_OF_ELEMENTS - i - 1];
		expected_results.data_from_Dequeue = (void *)&array_of_erease[NUM_OF_ELEMENTS - i - 1];

		results.is_empty = PQIsEmpty(pqueue);
		results.size = PQSize(pqueue);
		results.data_from_peek = PQPeek(pqueue);
		results.data_from_Dequeue = PQErease(pqueue, Cmp, (void *)&array_of_erease[NUM_OF_ELEMENTS - i - 1]);

		test_flag += TestResultComperMulti(results, expected_results, test_name, __LINE__);			                  
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	PQDestroy(pqueue);
}
/*******************************************************************************
************************************** Cmp ************************************/

static int Cmp(const void *data, const void *param)
{
	assert(param);
	assert(data);

	return (*(int *)param - *(int *)data);
}

/*******************************************************************************
****************************** dlist Multi Insert *****************************/

static void PQueueMultiInsert(pqueue_ty *pqueue, size_t num_of_insert, 
                                                                     int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		PQEnqueue(pqueue, (void *)(array + i));
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
	if(*(int *)(results.data_from_Dequeue) != *(int *)(expected_results.data_from_Dequeue))
	{
		printf(RED);
		printf("\nThe Sequence of Dequeue's test fail for %s %s at line %lu\n", 
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(*(int *)(results.data_from_peek) != *(int *)(expected_results.data_from_peek))
	{
		printf(RED);
		printf("\nThe Sequence of peek's test fail for %s %s at line %lu\n",
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
**************************** Test Result Comper Multi *************************/

static int TestResultComperMulti(results_ty results, results_ty expected_results,
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
	if(*(int *)(results.data_from_Dequeue) != *(int *)(expected_results.data_from_Dequeue))
	{
		printf(RED);
		printf("\nThe Sequence of Dequeue's test fail for %s %s at line %lu\n", 
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(*(int *)(results.data_from_peek) != *(int *)(expected_results.data_from_peek))
	{
		printf(RED);
		printf("\nThe Sequence of peek's test fail for %s %s at line %lu\n",
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	} 

	return test_is_successful_flag;
}

/*******************************************************************************
************************ Test Print Result Comper Multi ***********************/

static void TestPrintResultComperMulti(int test_is_successful_flag, const char *test_name)
{ 
	const char *test_object= "pqueue test";

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








