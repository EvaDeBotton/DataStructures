/******************************************************************************
* Sorted fixed_size_allocator_test.c                                                     *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Nasrat                                                              *
*                                                                             *
******************************************************************************/ 
#include <stdio.h>  /* size_t */
#include <stdlib.h> /* free, malloc */

#include "fixed_size_allocator.h"
#include "utils.h"

#define FAILUER               1
#define SUCCESS               0 
#define COUNTER_EMPTY         0 
#define NUM_OF_BLOCKS        10

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) {(type *)malloc(sizeof(type) * nelements)};

typedef struct results
{  
	size_t suggested_size;
	size_t free_blocks;		
    void * alloc_ptr;
	
} results_ty;

/* test functions */
void TestPoolInit(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
/* helper */
/*
static void SchedulerMultiInsert(scheduler_ty *scheduler, size_t num_of_insert, 
                                                                    int *array);
*/
/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Fixed Size Allocator Test";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestPoolInit();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Test Initialize Pool *****************************/

/*	test case: checking all the list parameter after initializing
*/
void TestPoolInit()
{ 
	const char *test_name = "INITIALIZE & SUGGESTED_SIZE";

	size_t block_size = 10;
	size_t num_of_blocks =10;
	size_t pool_size = 168; 

	void *memory_pool = MALLOC(pool_size, char) 

	pool_ty *pool = PoolInit(memory_pool, pool_size, block_size);
	
	results_ty expected_results;
	results_ty results;

	expected_results.suggested_size = pool_size;
	expected_results.free_blocks = num_of_blocks;
	expected_results.alloc_ptr = memory_pool;

	results.suggested_size = SuggestedSize(block_size, num_of_blocks);
	results.free_blocks = PoolFreeBlocks(pool);
	results.alloc_ptr = pool;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	FREE(memory_pool)
}

/*******************************************************************************
******************************** Test Full Pool *******************************/

/*	test case: checking all the list parameter after initializing
*/
/*
void TestFullPool()
{ 
	const char *test_name = "INITIALIZE & SUGGESTED_SIZE";

	size_t block_size = 10;
	size_t pool_size = 100; 

	void *memory_pool = MALLOC(pool_size, char) 

	pool_ty *pool = PoolInit(memory_pool, pool_size, block_size);
	
	results_ty expected_results;
	results_ty results;

	expected_results.suggested_size = ;
	expected_results.free_blocks = ;
	expected_results.alloc_ptr = memory_pool;

	results.suggested_size = SuggestedSize(block_size, num_of_blocks);
	results.free_blocks = PoolFreeBlocks(pool);
	results.alloc_ptr = pool;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	FREE(memory_pool)
}*/

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "Fixed Size Allocator Test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.suggested_size  != expected_results.suggested_size )
	{
		printf(RED);
		printf("\nThe suggested_size test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.free_blocks != expected_results.free_blocks)
	{
		printf(RED);
		printf("\nThe free_blocks test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.alloc_ptr != expected_results.alloc_ptr )
	{
		printf(RED);
		printf("\nThe alloc_ptr test fail for %s %s at line %lu\n", 
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








