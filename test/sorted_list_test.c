/******************************************************************************
* Sorted List_test.c                                                          *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Lial                                                                *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
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

typedef struct fun_results
{  
	int status;
	dlist_iter_ty return_ptr;

} fun_results_ty;

/* test functions */
void TestInitializeDoubleList(void);
void TestFullDoubleList(void);
void TestPopsDoubleList(void);
void TestMergeDoubleList(void);
void TestSpliceDoubleList(void); 
void TestMultiFindFunctionDoubleList(void);
void TestForEachFunction(void);


/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static int CheckSortedListSequenceOfNext(sorted_list_ty *list,int *array_of_data
	                                                             , size_t size);
static int CheckSortedListSequenceOfPrev(sorted_list_ty *list,int *array_of_data
	                                                             , size_t size);
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void FunResultComper(fun_results_ty fun_results, 
	   fun_results_ty expected_fun_results, const char *test_name, size_t line);

/* helper */
/*static sorted_list_iter_ty DListMultiNext(sorted_list_iter_ty iter, size_t num_of_next);
static sorted_list_iter_ty DListMultiPrev(sorted_list_iter_ty iter, size_t num_of_next);*/
static void SortedListMultiInsert(sorted_list_ty *list, size_t num_of_insert, 
                                                                    int *array);

/* action & match functions */
static int Cmp(const void *data, const void *param);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Double List Tests";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestInitializeDoubleList();
	TestFullDoubleList();
	TestPopsDoubleList();
	TestMergeDoubleList();
	/*TestSpliceDoubleList();
	TestMultiFindFunctionDoubleList();
	TestForEachFunction();*/
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
************************* Test Initialize Double List *************************/

/*	test case: checking all the dlist parameter after initializing
*/
void TestInitializeDoubleList(void)
{ 
	const char *test_name = "INITIALIZE";

	sorted_list_ty *list = SortedListCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = SUCSSES;
	results.prev = SUCSSES;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	SortedListDestroy(list);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the dlist parameter after inserting n elements
*/

void TestFullDoubleList(void)
{ 
	const char *test_name = "FULL";

	sorted_list_ty *list = SortedListCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[10] = {1,2,3,4,5,6,7,8,9,10};

	SortedListMultiInsert(list, NUM_OF_ELEMENTS, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = CheckSortedListSequenceOfNext(list, array_of_data, NUM_OF_ELEMENTS);
	results.prev = CheckSortedListSequenceOfPrev(list, array_of_data, NUM_OF_ELEMENTS);

	TestResultComper(results, expected_results, test_name, __LINE__);

	SortedListDestroy(list);
}

/*******************************************************************************
*************************** Test Pops Double List ***************************/

/*	test case: checking all the src and dest (dlist) parameters after inserting 
    n elements to the src dlist, and splice m element into the dest dlist. 
*/

void TestPopsDoubleList(void)
{ 
	const char *test_name_pop_front = "POP FRONT";
	const char *test_name_pop_back = "POP BACK";
	const char *test_name_remove = "REMOVE";
	const char *test_name_empty = "EMPTY";

	sorted_list_ty *list = SortedListCreate(Cmp);  

	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,2,3,4,5,6,7,8,9,10};
	int array_pop_front[SEVEN] = {4,5,6,7,8,9,10};
	int array_pop_back[FOUR] = {4,5,6,7}; 
	int array_remove[ONE] = {7};
	int i = 0; 

	SortedListMultiInsert(list, NUM_OF_ELEMENTS, array_of_data);

	/*______________________________ pop front ______________________________ */

	for(i = 0; i < THREE; i++)
	{
		SortedListPopFront(list);
	}

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = SEVEN;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = CheckSortedListSequenceOfNext(list, array_pop_front, SEVEN);
	results.prev = CheckSortedListSequenceOfPrev(list, array_pop_front, SEVEN);
		
	TestResultComper(results, expected_results, test_name_pop_front, __LINE__);

	/*_______________________________ pop back ______________________________ */

	for(i = 0; i < THREE; i++)
	{
		SortedListPopBack(list);
	}

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = FOUR;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = CheckSortedListSequenceOfNext(list, array_pop_back, FOUR);
	results.prev = CheckSortedListSequenceOfPrev(list, array_pop_back, FOUR);

	TestResultComper(results, expected_results, test_name_pop_back, __LINE__);

	/*________________________________ remove _______________________________ */

	for(i = 0; i < THREE; i++)
	{
		SortedListRemove(SortedListBegin(list));
	}
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = ONE;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = CheckSortedListSequenceOfNext(list, array_remove, ONE);
	results.prev = CheckSortedListSequenceOfPrev(list, array_remove, ONE);

	TestResultComper(results, expected_results, test_name_remove, __LINE__);

	/*________________________________ empty ________________________________ */


	SortedListRemove(SortedListBegin(list));
	
	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(list);
	results.size = SortedListSize(list);
	results.next = SUCSSES;
	results.prev = SUCSSES;

	TestResultComper(results, expected_results, test_name_empty, __LINE__);

	SortedListDestroy(list);
}

/*******************************************************************************
**************************** Test Merge Sorted List ****************************/

/*	test case: checking all the dlist parameter after inserting n elements
*/

void TestMergeDoubleList(void)
{ 
	const char *test_name = "MERGE";

	sorted_list_ty *src = SortedListCreate(Cmp);
	sorted_list_ty *dest = SortedListCreate(Cmp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data_src[10] = {3,10,20,30,40,50,60,70,80,100};
	int array_of_data_dest[10] = {1,7,9,25,41,42,43,100,200,300};
	int array_of_data_merge[20] = {1,3,7,9,10,20,25,30,40,41,42,
		                           43,50,60,70,80,100,100,200,300};

	SortedListMultiInsert(src, NUM_OF_ELEMENTS, array_of_data_src);
	SortedListMultiInsert(dest, NUM_OF_ELEMENTS, array_of_data_dest);

	SortedListMerge(dest, src);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = 2*NUM_OF_ELEMENTS;
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = SortedListIsEmpty(dest);
	results.size = SortedListSize(dest);
	results.next = CheckSortedListSequenceOfNext(dest, array_of_data_merge, 
		                                                     2*NUM_OF_ELEMENTS);
	results.prev = CheckSortedListSequenceOfPrev(dest, array_of_data_merge, 
		                                                     2*NUM_OF_ELEMENTS);

	TestResultComper(results, expected_results, test_name, __LINE__);

	SortedListDestroy(dest);
	SortedListDestroy(src);
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

static void SortedListMultiInsert(sorted_list_ty *list, size_t num_of_insert, 
                                                                     int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		SortedListInsert(list, (void *)(array + i));
	}
}

/*******************************************************************************
************************** Check Dlist Sequence Of Next ***********************/

static int CheckSortedListSequenceOfNext(sorted_list_ty *list, int *array_of_data
	                                                              , size_t size)
{
	sorted_list_iter_ty iter = {0};
	int status = SUCSSES;
	const void *data = NULL;
	size_t i = 0;  

	iter = SortedListBegin(list);
	data = SortedListGetData(iter);

	/* testing the next type of the nodes */
	for(i = 0 ; i < size ; i++)
	{
		if(*(int *)(data) != *(array_of_data + i))
		{
			status = FAILURE; 
			break; 
		}

		iter = SortedListNext(iter);
		data = SortedListGetData(iter); 
	}
	return status;
}

/*******************************************************************************
************************** Check Dlist Sequence Of Prev ***********************/

static int CheckSortedListSequenceOfPrev(sorted_list_ty *list, int *array_of_data
	                                                              , size_t size)
{
	sorted_list_iter_ty iter = {0};
	int status = SUCSSES;
	void *data = NULL;
	size_t i = 0;  

	iter = SortedListPrev(SortedListEnd(list));
	data = SortedListGetData(iter);

	/* testing the next type of the nodes */
	for(i = 0 ; i < size ; i++)
	{
		if(*(int *)(data) != *(array_of_data + size - i - 1))
		{
			status = FAILURE; 
			break; 
		}

		iter = SortedListPrev(iter);
		data = SortedListGetData(iter); 
	}
	return status;
}
/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "sorted list test";
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
	/*if(results.begin != expected_results.begin)
	{
		printf(RED);
		printf("\nThe results.begin test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.end != expected_results.end)
	{
		printf(RED);
		printf("\nThe results.end test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	} */
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
******************************* Test Result Comper ****************************/

static void FunResultComper(fun_results_ty fun_results, 
	    fun_results_ty expected_fun_results, const char *test_name, size_t line)
{ 
	const char *test_object= "function results";
	int test_is_successful_flag = ILRD_TRUE;

	if(expected_fun_results.status != fun_results.status)
	{
		printf(RED);
		printf("\nThe status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(expected_fun_results.return_ptr != fun_results.return_ptr)
	{
		printf(RED);
		printf("\nThe init counter test fail for %s %s at line %lu\n",
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








