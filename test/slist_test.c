/******************************************************************************
* slist_test.c                                                                *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd                                                                     *
*                                                                             *
******************************************************************************/ 

#include <stdio.h> /* size_t */
#include <string.h> /* strcmp, strcmp */
#include <assert.h> /* assert */
#include "slist.h"

/* colors for debugging */
#define RED			                     "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"

#define EMPTY                                       1
#define NOT_EMPTY                                   0
#define COUNTER_EMPTY                               0
#define ONE                                         1                    

typedef struct results
{  
	bool_ty is_empty;
	size_t counter;
	slist_iter_ty begin;
    slist_iter_ty end;		
    void *get_data;
	
} results_ty;

struct slist
{
	slist_iter_ty head; 
	slist_iter_ty tail; 
};

struct slist_node
{
	void *data; 
	slist_iter_ty next; 
};

void InitializeSlistTests(void);
void FullSlistTests(void); 
void SlistSetDataTests(void);
void EmptySlistTests(void);
void EndOfTests(void);

/* helper */
static void SlistMultiInsert(slist_iter_ty iter, size_t num_of_insert, 
                                                                 void *element);
static void SlistMultiRemove(slist_iter_ty iter, size_t num_of_remove);

static void SlistMultiSet(slist_iter_ty iter, size_t num_of_set, void *data);

static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static slist_iter_ty SlistMultiNext(slist_iter_ty iter, size_t num_of_next);

static action_func_ty ChangeData; 

static bool_ty PhoneNumMatch(const void *num_1, void *num_2);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	InitializeSlistTests();
	FullSlistTests();
	SlistSetDataTests();
	EmptySlistTests();
	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Initialize Slist Test ****************************/

/*	test case: checking all the Slist parameter after initializing
*/
void InitializeSlistTests(void)
{ 
	slist_ty *slist = SlistCreate();
	const char *test_name = "Initialize Slist";
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.counter = COUNTER_EMPTY;
	expected_results.begin = SlistNext(slist->head);
	expected_results.end = SlistNext(slist->head);
	expected_results.get_data = NULL;

	results.is_empty = SlistIsEmpty(slist);
	results.counter = SlistCount(slist);
	results.begin = SlistBegin(slist);
	results.end = SlistEnd(slist);
	results.get_data = NULL;
	
	SlistDestroy(slist);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************************* Full Slist Test *******************************/

/*	test case: checking all the Slist parameter after inserting n elements
*/
void FullSlistTests(void)
{ 
	slist_ty *slist = SlistCreate();
	const char *test_name = "Full Slist";

	results_ty expected_results;
	results_ty results;

	char *test = "hello world"; 
	void *ptr_to_test = &test;
	size_t num_of_inserts = 2; 

	SlistMultiInsert(SlistEnd(slist), num_of_inserts, ptr_to_test);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.counter = num_of_inserts;
	expected_results.begin = SlistNext(slist->head);
	expected_results.end = SlistMultiNext(SlistBegin(slist),num_of_inserts);
	expected_results.get_data = NULL;

	results.is_empty = SlistIsEmpty(slist);
	results.counter = SlistCount(slist);
	results.begin = SlistBegin(slist);
	results.end = SlistEnd(slist);
	results.get_data = NULL;
	/*TODO: check the data of inserted elements */
	SlistDestroy(slist);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
**************************** Slist Set Data Tests *****************************/

/*	test case: checking all the Slist parameter after inserting n elements and 
	           changing one element with set data. 
*/
void SlistSetDataTests(void)
{ 
	slist_ty *slist = SlistCreate();
	const char *test_name = "Set Data";

	results_ty expected_results;
	results_ty results;

	char *test_1 = "hello world"; 
	int   test_2 = 6;
	void *ptr_to_test_1 = &test_1;
	void *ptr_to_test_2 = &test_2;
	size_t num_of_inserts = 10; 

	SlistMultiInsert(SlistEnd(slist), num_of_inserts, ptr_to_test_1);
	SlistSetData(SlistBegin(slist), ptr_to_test_2);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.counter = num_of_inserts;
	expected_results.begin = SlistNext(slist->head);
	expected_results.end = SlistMultiNext(SlistBegin(slist),num_of_inserts);
	expected_results.get_data = ptr_to_test_2;

	results.is_empty = SlistIsEmpty(slist);
	results.counter = SlistCount(slist);
	results.begin = SlistBegin(slist);
	results.end = SlistEnd(slist);
	results.get_data = SlistGetData(SlistBegin(slist));
	
	SlistDestroy(slist);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
***************************** Empty Slist Tests *******************************/

/*	test case: checking all the Slist parameter after inserting n elements and 
	           removing n element. 
*/
void EmptySlistTests(void)
{ 
	slist_ty *slist = SlistCreate();
	const char *test_name = "Empty Slist";

	results_ty expected_results;
	results_ty results;

	char *test = "hello world"; 
	void *ptr_to_test = &test;
	size_t num_of_inserts = 10; 

	SlistMultiInsert(SlistEnd(slist), num_of_inserts, ptr_to_test);
	SlistMultiRemove(SlistBegin(slist), num_of_inserts);

	expected_results.is_empty = EMPTY;
	expected_results.counter = COUNTER_EMPTY;;
	expected_results.begin = SlistNext(slist->head);
	expected_results.end = SlistBegin(slist);
	expected_results.get_data = NULL;

	results.is_empty = SlistIsEmpty(slist);
	results.counter = SlistCount(slist);
	results.begin = SlistBegin(slist);
	results.end = SlistEnd(slist);
	results.get_data = NULL;
	
	SlistDestroy(slist);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
****************************** Slist Multi Insert *****************************/

static void SlistMultiInsert(slist_iter_ty iter, size_t num_of_insert, 
                                                                     int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		iter = SlistInsert((void *)(array + i), iter);
		assert(NULL != iter);
	}
}

/*******************************************************************************
****************************** Slist Multi Remove *****************************/

static void SlistMultiRemove(slist_iter_ty iter, size_t num_of_remove)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_remove ; i++)
	{
		assert(iter->next != NULL);
		SlistRemove(iter);
	}
}


/*******************************************************************************
******************************* Slist Multi Set *******************************/

static void SlistMultiSet(slist_iter_ty iter, size_t num_of_set, void *data)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_set; i++)
	{
		SlistSetData(iter, data);
		assert(NULL != SlistNext(iter));
		iter = SlistNext(iter);
	}
}

/*******************************************************************************
****************************** Slist Multi next *******************************/

static slist_iter_ty SlistMultiNext(slist_iter_ty iter, size_t num_of_next)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_next; i++)
	{
		iter = SlistNext(iter);
	}

	return iter;
}

/*******************************************************************************
******************************** Phone Num Match*******************************/

static bool_ty PhoneNumMatch(const void *num_1, void *num_2) 
{
	return ((*(int *)num_1 == *(int *)num_2) ? 1 : 0);
}  

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line)
{ 
	if(results.is_empty != expected_results.is_empty)
	{
		printf(RED);
		printf("\nThe is_empty test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.counter != expected_results.counter)
	{
		printf(RED);
		printf("\nThe init counter test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.begin != expected_results.begin)
	{
		printf(RED);
		printf("\nThe results.begin test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.end != expected_results.end)
	{
		printf(RED);
		printf("\nThe results.end test fail for %s test at line %lu\n",
		                                                       test_name, line);
	} 
	if(results.get_data != expected_results.get_data)
	{
		printf(RED);
		printf("\nThe get_data test fail for %s test at line %lu\n",
		                                                       test_name, line);
	} 
}

/*******************************************************************************
********************************** End Of Tests *******************************/

void EndOfTests(void)
{
	printf(YELLOW);
	printf("\n_________________________End Of Tests________________________\n");
}








