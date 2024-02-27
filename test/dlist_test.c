/******************************************************************************
* dlist_test.c                                                                *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                                 *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
#include "dlist.h"
#include "utils.h"

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10
#define SRC_SIZE              7 
#define DEST_SIZE             3
              
typedef struct results
{  
	int is_empty;
	size_t size;
	dlist_iter_ty begin;
    dlist_iter_ty end;		
    int next;
    int prev;
	
} results_ty;

typedef struct fun_results
{  
	int status;
	dlist_iter_ty return_ptr;

} fun_results_ty;

struct dlist
{
	dlist_iter_ty head; 
	dlist_iter_ty tail; 
};

struct dlist_node
{
	void *data; 
	dlist_iter_ty next; 
};

enum compare
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

/* test functions */
void TestInitializeDoubleList(void);
void TestFullDoubleList(void);
void TestPopsDoubleList(void);
void TestSpliceDoubleList(void); 
void TestMultiFindFunctionDoubleList(void);
void TestForEachFunction(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static int CheckDlistSequenceOfNext(dlist_ty *dlist, int *array, size_t size);
static int CheckDlistSequenceOfPrev(dlist_ty *dlist, int *array, size_t size);
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void FunResultComper(fun_results_ty fun_results, 
	   fun_results_ty expected_fun_results, const char *test_name, size_t line);

/* helper */
static dlist_iter_ty DListMultiNext(dlist_iter_ty iter, size_t num_of_next);
static dlist_iter_ty DListMultiPrev(dlist_iter_ty iter, size_t num_of_next);
static void DListMultiInsert(dlist_iter_ty iter, size_t num_of_insert, int *array);

/* action & match functions */
static int AddN(void *data, void *Number); 
static int IsDividesByN(const void *data, void *number);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Double List Tests";

	BeginingOfTests(tests_name);

	/* The main test functions*/ 
	TestInitializeDoubleList();
	TestFullDoubleList();
	TestPopsDoubleList();
	TestSpliceDoubleList();
	TestMultiFindFunctionDoubleList();
	TestForEachFunction();
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

	dlist_ty *dlist = DListCreate();
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.begin = DListNext(dlist->head);
	expected_results.end = DListNext(dlist->head);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = SUCSSES;
	results.prev = SUCSSES;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	DListDestroy(dlist);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the dlist parameter after inserting n elements
*/

void TestFullDoubleList(void)
{ 
	const char *test_name = "FULL";

	dlist_ty *dlist = DListCreate();
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[10] = {1,2,3,4,5,6,7,8,9,10};

	DListMultiInsert(DListEnd(dlist), NUM_OF_ELEMENTS, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.begin = DListMultiPrev(DListEnd(dlist),NUM_OF_ELEMENTS);
	expected_results.end = DListMultiNext(DListBegin(dlist),NUM_OF_ELEMENTS);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = CheckDlistSequenceOfNext(dlist, array_of_data, NUM_OF_ELEMENTS);
	results.prev = CheckDlistSequenceOfPrev(dlist, array_of_data, NUM_OF_ELEMENTS);

	DListDestroy(dlist);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
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

	dlist_ty *dlist = DListCreate();  

	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,2,3,4,5,6,7,8,9,10};
	int array_pop_front[SEVEN] = {4,5,6,7,8,9,10};
	int array_pop_back[FOUR] = {4,5,6,7}; 
	int array_remove[ONE] = {7};
	int i = 0; 

	DListMultiInsert(DListEnd(dlist), NUM_OF_ELEMENTS, array_of_data);

	/*______________________________ pop front ______________________________ */

	for(i = 0; i < THREE; i++)
	{
		DListPopFront(dlist);
	}

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = SEVEN;
	expected_results.begin = DListMultiPrev(DListEnd(dlist), SEVEN);
	expected_results.end = DListMultiNext(DListBegin(dlist), SEVEN);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = CheckDlistSequenceOfNext(dlist, array_pop_front, SEVEN);
	results.prev = CheckDlistSequenceOfPrev(dlist, array_pop_front, SEVEN);
		
	TestResultComper(results, expected_results, test_name_pop_front, __LINE__);

	/*_______________________________ pop back ______________________________ */

	for(i = 0; i < THREE; i++)
	{
		DListPopBack(dlist);
	}

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = FOUR;
	expected_results.begin = DListMultiPrev(DListEnd(dlist),FOUR);
	expected_results.end = DListMultiNext(DListBegin(dlist),FOUR);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = CheckDlistSequenceOfNext(dlist, array_pop_back, FOUR);
	results.prev = CheckDlistSequenceOfPrev(dlist, array_pop_back, FOUR);

	TestResultComper(results, expected_results, test_name_pop_back, __LINE__);

	/*________________________________ remove _______________________________ */

	for(i = 0; i < THREE; i++)
	{
		SortedListRemove(DListBegin(dlist));
	}
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = ONE;
	expected_results.begin = DListMultiPrev(DListEnd(dlist),ONE);
	expected_results.end = DListMultiNext(DListBegin(dlist),ONE);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = CheckDlistSequenceOfNext(dlist, array_remove, ONE);
	results.prev = CheckDlistSequenceOfPrev(dlist, array_remove, ONE);

	TestResultComper(results, expected_results, test_name_remove, __LINE__);

	/*________________________________ empty ________________________________ */


	SortedListRemove(DListBegin(dlist));
	
	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.begin = DListNext(dlist->head);
	expected_results.end = DListNext(dlist->head);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = SUCSSES;
	results.prev = SUCSSES;

	TestResultComper(results, expected_results, test_name_empty, __LINE__);

	DListDestroy(dlist);
}

/*******************************************************************************
*************************** Test Splice Double List ***************************/

/*	test case: checking all the src and dest (dlist) parameters after inserting 
    n elements to the src dlist, and splice m element into the dest dlist. 
*/

void TestSpliceDoubleList(void)
{ 
	const char *test_name_src = "SPLICE - SRC";
	const char *test_name_dest = "SPLICE -DEST";

	dlist_ty *src = DListCreate();  
	dlist_ty *dest = DListCreate();

	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,2,3,4,5,6,7,8,9,10};
	int array_of_data_dest[DEST_SIZE] = {1,2,3};
	int array_of_data_src[SRC_SIZE] = {4,5,6,7,8,9,10}; 

	DListMultiInsert(DListEnd(src), NUM_OF_ELEMENTS, array_of_data);
	DListSplice(DListBegin(src), DListMultiNext(DListBegin(src), DEST_SIZE)              
		                                           , DListPrev(DListEnd(dest)));

	/*______________________________ src check ______________________________ */

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = SRC_SIZE;
	expected_results.begin = DListMultiPrev(DListEnd(src), SRC_SIZE);
	expected_results.end = DListMultiNext(DListBegin(src), SRC_SIZE);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(src);
	results.size = DListSize(src);
	results.begin = DListBegin(src);
	results.end = DListEnd(src);
	results.next = CheckDlistSequenceOfNext(src, array_of_data_src, SRC_SIZE);
	results.prev = CheckDlistSequenceOfPrev(src, array_of_data_src, SRC_SIZE);
		
	TestResultComper(results, expected_results, test_name_src, __LINE__);

	/*______________________________ dest check _____________________________ */

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = DEST_SIZE;
	expected_results.begin = DListMultiPrev(DListEnd(dest),DEST_SIZE);
	expected_results.end = DListMultiNext(DListBegin(dest),DEST_SIZE);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dest);
	results.size = DListSize(dest);
	results.begin = DListBegin(dest);
	results.end = DListEnd(dest);
	results.next = CheckDlistSequenceOfNext(dest, array_of_data_dest, DEST_SIZE);
	results.prev = CheckDlistSequenceOfPrev(dest, array_of_data_dest, DEST_SIZE);

	TestResultComper(results, expected_results, test_name_dest, __LINE__);

	DListDestroy(dest);
	DListDestroy(src);
}

/*******************************************************************************
************************** Test Multi Find Function ***************************/

/*	test case: checking all the src and dest (dlist) parameters after inserting 
    n elements to the src dlist, and splice m element into the dest dlist. 
*/

void TestMultiFindFunctionDoubleList(void)
{ 
	const char *test_find_case_1 = "NO MATCH CASE - FIND FUNCTION";
	const char *test_multi_find_case_1 = "NO MATCH CASE - MULTI FIND FUNCTION";
	const char *test_find_case_2 = "MATCH CASE - FIND FUNCTION";
	const char *test_multi_find_case_2 = "THREE MATCH CASE - MULTI FIND FUNCTION";


	dlist_ty *dlist = DListCreate(); 
	dlist_ty *dest_case_1 = DListCreate(); 
	dlist_ty *dest_case_2 = DListCreate();

	dlist_iter_ty from_case_1 = NULL;
	dlist_iter_ty from_case_2 = NULL;
	dlist_iter_ty to_case_1 = NULL; 
	dlist_iter_ty to_case_2 = NULL;

	results_ty expected_results;
	results_ty results;

	fun_results_ty expected_fun_results;
	fun_results_ty fun_results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,3,5,7,9,2,4,3,8,1};
	int array_of_data_match[THREE] = {2,4,8};
	int param = 2;


	DListMultiInsert(DListEnd(dlist), NUM_OF_ELEMENTS, array_of_data);

	/*______________________________ case 1 ________________________________ */

	from_case_1 = DListBegin(dlist);
	
	to_case_1 = DListMultiNext(DListBegin(dlist), THREE);

	fun_results.return_ptr = DListFind(from_case_1, to_case_1, IsDividesByN, 
		                                                        (void *)&param);
	fun_results.status = DListMultiFind(from_case_1, to_case_1, IsDividesByN,
	                                               (void *)&param, dest_case_1);
	expected_fun_results.status = SUCSSES;
	expected_fun_results.return_ptr = to_case_1;

	FunResultComper(fun_results, expected_fun_results, test_find_case_1, __LINE__);

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.begin = DListEnd(dest_case_1);
	expected_results.end = DListBegin(dest_case_1);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dest_case_1);
	results.size = DListSize(dest_case_1);
	results.begin = DListBegin(dest_case_1);
	results.end = DListEnd(dest_case_1);
	results.next = SUCSSES;
	results.prev = SUCSSES;
		
	TestResultComper(results, expected_results, test_multi_find_case_1, __LINE__);

	/*______________________________ case 2 ________________________________ */

	from_case_2 = DListBegin(dlist);
	to_case_2 = DListEnd(dlist);
	
	fun_results.return_ptr = DListFind(from_case_2, to_case_2, IsDividesByN, 
		                                                        (void *)&param);
	fun_results.status = DListMultiFind(from_case_2, to_case_2, IsDividesByN,
	                                               (void *)&param, dest_case_2);
	expected_fun_results.status = SUCSSES;
	expected_fun_results.return_ptr = DListMultiNext(DListBegin(dlist),FIVE);

	FunResultComper(fun_results, expected_fun_results, test_find_case_2, __LINE__);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = THREE;
	expected_results.begin = DListMultiPrev(DListEnd(dest_case_2),THREE);
	expected_results.end = DListMultiNext(DListBegin(dest_case_2), THREE);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dest_case_2);
	results.size = DListSize(dest_case_2);
	results.begin = DListBegin(dest_case_2);
	results.end = DListEnd(dest_case_2);
	results.next = CheckDlistSequenceOfNext(dest_case_2, array_of_data_match, THREE);
	results.prev = CheckDlistSequenceOfPrev(dest_case_2, array_of_data_match, THREE);

	TestResultComper(results, expected_results, test_multi_find_case_2, __LINE__);

	DListDestroy(dlist);
	DListDestroy(dest_case_1);
	DListDestroy(dest_case_2);
}

/*******************************************************************************
**************************** Test For Each Function ***************************/

/*	test case: checking all the src and dest (dlist) parameters after inserting 
    n elements to the src dlist, and splice m element into the dest dlist. 
*/

void TestForEachFunction(void)
{ 
	const char *test_name = "FOR EACH";

	dlist_ty *dlist = DListCreate(); 

	dlist_iter_ty from = NULL;
	dlist_iter_ty to = NULL;  

	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,3,5,7,9,2,4,3,8,1};
	int array_of_data_after[NUM_OF_ELEMENTS] = {11,13,15,7,9,2,4,3,8,1};
	int param = 10;

	DListMultiInsert(DListEnd(dlist), NUM_OF_ELEMENTS, array_of_data);

	from = DListBegin(dlist);	
	to = DListMultiNext(DListBegin(dlist), THREE);

	DListForEach(from, to, AddN, (void *)&param);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.begin = DListMultiPrev(DListEnd(dlist), NUM_OF_ELEMENTS);
	expected_results.end = DListMultiNext(DListBegin(dlist), NUM_OF_ELEMENTS);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = DListIsEmpty(dlist);
	results.size = DListSize(dlist);
	results.begin = DListBegin(dlist);
	results.end = DListEnd(dlist);
	results.next = CheckDlistSequenceOfNext(dlist, array_of_data_after, NUM_OF_ELEMENTS);
	results.prev = CheckDlistSequenceOfPrev(dlist, array_of_data_after, NUM_OF_ELEMENTS);;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	DListDestroy(dlist);
}

/*******************************************************************************
****************************** dlist Multi Insert *****************************/

static void DListMultiInsert(dlist_iter_ty iter, size_t num_of_insert, 
                                                                     int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		DListInsert(iter, (void *)(array + i));
		assert(NULL != iter);
	}
}

/*******************************************************************************
****************************** dlist Multi next *******************************/

static dlist_iter_ty DListMultiNext(dlist_iter_ty iter, size_t num_of_next)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_next; i++)
	{
		iter = DListNext(iter);
	}

	return iter;
}

/*******************************************************************************
****************************** dlist Multi prev *******************************/

static dlist_iter_ty DListMultiPrev(dlist_iter_ty iter, size_t num_of_next)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_next; i++)
	{
		iter = DListPrev(iter);
	}

	return iter;
} 

/*******************************************************************************
************************** Check Dlist Sequence Of Next ***********************/

static int CheckDlistSequenceOfNext(dlist_ty *dlist, int *array_of_data
	                                                              , size_t size)
{
	int status = SUCSSES;
	size_t i = 0;  

	dlist_iter_ty iter = DListBegin(dlist);

	/* testing the next type of the nodes */
	for(i = 0 ; i < size ; i++)
	{
		if(*(int *)(iter->data) != *(array_of_data + i))
		{
			status = FAILURE; 
			break; 
		}

		iter = DListNext(iter); 
	}
	return status;
}

/*******************************************************************************
********************************* Is Divides By N *****************************/

static int IsDividesByN(const void *data, void *number)
{
	return ( 0 == *(int *)data % *(int *)number);
}

/*******************************************************************************
************************************* Add N ***********************************/

static int AddN(void *data, void *number)
{
	*(int *)data += *(int *)number;
	
	return SUCSSES;
}

/*******************************************************************************
************************** Check Dlist Sequence Of Prev ***********************/

static int CheckDlistSequenceOfPrev(dlist_ty *dlist, int *array_of_data
	                                                              , size_t size)
{
	int status = SUCSSES;
	size_t i = 0;
	dlist_iter_ty iter = DListPrev(DListEnd(dlist));  

	array_of_data += size - 1; 

	/* testing the prev type of the nodes */
	for(i = 0 ; i < size ; i++)
	{
		if(*(int *)(iter->data) != *(array_of_data - i))
		{
			status = FAILURE; 
			break; 
		}

		iter = DListPrev(iter); 
	} 

	return status; 
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "double list test";
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
	if(results.begin != expected_results.begin)
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








