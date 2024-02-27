/******************************************************************************
* Sorted fixed_size_allocator_test.c                                          *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Doron                                                               *
*                                                                             *
******************************************************************************/ 
#include <stdio.h>  /* size_t */
#include <stdlib.h> /* free, malloc */

#include "fixed_size_allocator.h"
#include "utils.h"

#define NUM_OF_TESTS           5

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);

typedef struct results
{  
	size_t suggested_size;
	size_t free_blocks;		
    void * alloc_ptr;
	
} results_ty;

/* test functions */
void TestPoolInit(void);
void TestAllocFromPool(void);
void TestFreeToPool(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Fixed Size Allocator Test";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestPoolInit();
	TestAllocFromPool();
	TestFreeToPool();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Test Initialize Pool *****************************/

/*	test case: checking all the list parameter after initializing
*/
void TestPoolInit(void)
{ 
	const char *test_name = "INITIALIZE & SUGGESTED_SIZE";

	results_ty expected_results;
	results_ty results;

	size_t block_size[NUM_OF_TESTS] = {1, 8, 10, 16, 21};
	size_t num_of_blocks[NUM_OF_TESTS] = {10, 10, 5, 5, 1};
	size_t expected_suggested_size[NUM_OF_TESTS] = {88, 88, 88, 88, 32}; 

	size_t suggested_size = 0;
	size_t i = 0;

	void *memory_pool = NULL;
	fsa_ty *pool = {0};

	for(i = 0; i < NUM_OF_TESTS; i++)
	{
		suggested_size = FSASuggestedSize(block_size[i], num_of_blocks[i]);
		
		memory_pool = MALLOC(suggested_size, char) 

		pool = FSAInit(memory_pool, suggested_size, block_size[i]);
	
		expected_results.suggested_size = expected_suggested_size[i];
		expected_results.free_blocks = num_of_blocks[i];
		expected_results.alloc_ptr = memory_pool;
		
		results.suggested_size = suggested_size;
		results.free_blocks = FSAFreeBlocks(pool);
		results.alloc_ptr = pool;

		FREE(memory_pool)
		
		TestResultComper(results, expected_results, test_name, __LINE__);
	}

}

/*******************************************************************************
**************************** Test Alloc From Pool *****************************/

/*	test case: checking all the list parameter after initializing
*/
void TestAllocFromPool(void)
{ 
	const char *test_name = "ALLOC";

	results_ty expected_results;
	results_ty results;

	size_t block_size = 8;
	size_t num_of_blocks = 4;
	size_t expected_suggested_size = 40; 

	size_t i = 0;

	size_t suggested_size = FSASuggestedSize(block_size, num_of_blocks);
		
	char memory[40] = {0};
	void *memory_pool = memory;

	fsa_ty *pool = FSAInit(memory_pool, suggested_size, block_size);

	void *expected_alloc_ptr[NUM_OF_TESTS] = {NULL};

	for(i = 0; i < 4; i++)
	{
		expected_alloc_ptr[i] = ((char *)memory_pool + block_size *(i + 1));
	}

	for(i = 0; i < NUM_OF_TESTS; i++)
	{
		expected_results.suggested_size = expected_suggested_size;
		expected_results.free_blocks = num_of_blocks - i;
		expected_results.alloc_ptr = expected_alloc_ptr[i];

		results.suggested_size = suggested_size;
		results.free_blocks = FSAFreeBlocks(pool);
		results.alloc_ptr = FSAAlloc(pool);
		
		TestResultComper(results, expected_results, test_name, __LINE__);
	}

}

/*******************************************************************************
***************************** Test Free To Pool *******************************/

/*	test case: checking all the list parameter after initializing
*/
void TestFreeToPool(void)
{ 
	const char *test_name = "FREE";

	results_ty expected_results;
	results_ty results;

	size_t block_size = 8;
	size_t num_of_blocks = 4;
	size_t expected_suggested_size = 40; 

	size_t i = 0;

	size_t suggested_size = FSASuggestedSize(block_size, num_of_blocks);
		
	char memory[40] = {0};
	void *memory_pool = memory;

	fsa_ty *pool = FSAInit(memory_pool, suggested_size, block_size);

	void *expected_alloc_ptr[NUM_OF_TESTS] = {NULL};
	void *address_to_free[NUM_OF_TESTS] = {NULL};

	for(i = 0; i < 4; i++)
	{
		address_to_free[i] = ((char *)memory_pool + block_size *(i + 1));
	}

	for(i = 0; i < 4; i++)
	{
		expected_alloc_ptr[i] = ((char *)memory_pool + block_size *(i + 1));
	}

	for(i = 0; i < NUM_OF_TESTS; i++)
	{
		FSAAlloc(pool);
	}

	for(i = 0; i < NUM_OF_TESTS - 1; i++)
	{
		FSAFree(pool, address_to_free[i]);

		expected_results.suggested_size = expected_suggested_size;
		expected_results.free_blocks = 1;
		expected_results.alloc_ptr = expected_alloc_ptr[i];

		results.suggested_size = suggested_size;
		results.free_blocks = FSAFreeBlocks(pool);
		results.alloc_ptr = FSAAlloc(pool);
		
		TestResultComper(results, expected_results, test_name, __LINE__);
	}

}

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








