/******************************************************************************
* Sorted fixed_size_allocator_test.c                                          *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Bnaya                                                               *
*                                                                             *
******************************************************************************/ 
#include <stdio.h>  /* size_t */
#include <stdlib.h> /* free, malloc */

#include "variable_size_allocator.h"
#include "utils.h"

#define NUM_OF_TESTS            5
#define SIZE_ZERO               0
#define BYTES_IN_VSA_STRUCT     sizeof(vsa_ty)

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);

typedef struct results
{  
	size_t largest_free_block;		
    void * alloc_ptr;
	
} results_ty;

struct variable_size_allocator
{
    long block_size;
    /*debug_only(long id)*/
};

typedef struct ptr
{
    void *ptr;
}ptr_ty;

/* test functions */
void TestVSAInit(void);
void TestVSAAllocAndFreeSimpleCase(void);
void TestVSAAllocAndFreeMoreCases(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void BeginingOfTest(const char *tests_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Variable Size Allocator Test";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestVSAInit();
	TestVSAAllocAndFreeSimpleCase();
	TestVSAAllocAndFreeMoreCases();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Test Initialize VSA ******************************/

/*	test case: checking all the list parameter after initializing
*/
void TestVSAInit(void)
{ 
	const char *test_name = "Test Initialize VSA";

	const char *test_name_1 = "INITIALIZE MIN SIZE TEST";
	const char *test_name_2 = "INITIALIZE ALLINED SIZE TEST 1";
	const char *test_name_3 = "INITIALIZE NOT ALLINED TEST 1";
	const char *test_name_4 = "INITIALIZE ALLINED SIZE TEST 2";
	const char *test_name_5 = "INITIALIZE NOT ALLINED TEST 2";

	results_ty expected_results;
	results_ty results;

	size_t memory_size[NUM_OF_TESTS] = {24, 32, 35, 96, 100};
	size_t expected_largest_free_block[NUM_OF_TESTS] = {8, 16, 16, 80, 80}; 

	size_t i = 0;

	void *memory_vsa = NULL;
	vsa_ty *vsa = {0};

	BeginingOfTest(test_name);

/*_________________________________ min size _________________________________*/
		
	
	memory_vsa = MALLOC(memory_size[i], char) 
		
	vsa = VSAInit(memory_vsa, memory_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	FREE(memory_vsa)
		
	TestResultComper(results, expected_results, test_name_1, __LINE__);

	++i;

/*_______________________________ allined size 1 _____________________________*/

	memory_vsa = MALLOC(memory_size[i], char) 
		
	vsa = VSAInit(memory_vsa, memory_size[i]);
		
	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	FREE(memory_vsa)
		
	TestResultComper(results, expected_results, test_name_2, __LINE__);

	++i;

/*_____________________________ not allined size 1 ___________________________*/

	memory_vsa = MALLOC(memory_size[i], char) 
		
	vsa = VSAInit(memory_vsa, memory_size[i]);
		
	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	FREE(memory_vsa)
		
	TestResultComper(results, expected_results, test_name_3, __LINE__);

	++i;

/*_______________________________ allined size 2 _____________________________*/

	memory_vsa = MALLOC(memory_size[i], char) 
		
	vsa = VSAInit(memory_vsa, memory_size[i]);
		
	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	FREE(memory_vsa)
		
	TestResultComper(results, expected_results, test_name_4, __LINE__);

	++i;

/*_____________________________ not allined size 2 ___________________________*/

	memory_vsa = MALLOC(memory_size[i], char) 
		
	vsa = VSAInit(memory_vsa, memory_size[i]);
		
	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	FREE(memory_vsa)
		
	TestResultComper(results, expected_results, test_name_5, __LINE__);
	
}

/*******************************************************************************
**************************** Test Alloc From Pool *****************************/

/*	test case: checking the list parameter after free and alloc, all the space.
*/
void TestVSAAllocAndFreeSimpleCase(void)
{ 
	const char *test_name = "Test simple alloc & free";

	const char *test_name_alloc = "ALLOC ALL THE SPEACE TEST";
	const char *test_name_free  = "FREE  ALL THE SPEACE TEST";

	results_ty expected_results;
	results_ty results;

	size_t memory_size = 100;
	size_t block_size = 80;
	size_t expected_largest_free_block = 80; 

	void *memory_vsa = MALLOC(memory_size, char) 
	void *block_ptr = NULL;

	vsa_ty *vsa = VSAInit(memory_vsa, memory_size);

	BeginingOfTest(test_name);


/*____________________________ alloc all the space ___________________________*/

	
	block_ptr = VSAAlloc(vsa, block_size);
		
	expected_results.largest_free_block = SIZE_ZERO;
	expected_results.alloc_ptr = (void *)((char *)memory_vsa 
		                                                 + BYTES_IN_VSA_STRUCT);
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr;
		
	TestResultComper(results, expected_results, test_name_alloc , __LINE__);


/*____________________________ free all the space ____________________________*/

	VSAFree(block_ptr);

	expected_results.largest_free_block = expected_largest_free_block;
	expected_results.alloc_ptr = memory_vsa;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = vsa;

	TestResultComper(results, expected_results, test_name_free, __LINE__);

	FREE(memory_vsa)
}

/*******************************************************************************
**************************** Test Alloc From Pool *****************************/

/*	test case: checking the list parameter after free and alloc, diffrent size.
*/
void TestVSAAllocAndFreeMoreCases(void)
{ 
	const char *test_name = "Test more cases alloc & free";

	const char *test_name_1 = "ALLOC SMALLER THEN MEMORY & NOT ALLINED";
	const char *test_name_2 = "ALLOC SMALLER THEN MEMORY BUT BIGGER THEN LEFT";
	const char *test_name_3 = "ALLOC SMALLER THEN LEFT & ALINED";
	const char *test_name_4 = "ALLOC AFTER FREE AND DEFREGMENT";
	const char *test_name_5 = "ALLOC AFTER FREE ALL AND DEFREGMENT";

	results_ty expected_results;
	results_ty results;

	size_t memory_size = 100;
	size_t block_size[NUM_OF_TESTS] = {23, 50, 24, 40, 70};
	size_t block_size_allined[NUM_OF_TESTS] = {24, 56, 24, 40, 72};
	size_t expected_largest_free_block[NUM_OF_TESTS] = {48, 48, 16, 0, 0}; 

	size_t i = 0;

	void *memory_vsa = MALLOC(memory_size, char)

	vsa_ty *vsa = VSAInit(memory_vsa, memory_size);

	void *block_ptr[NUM_OF_TESTS] = {NULL};

	BeginingOfTest(test_name);

/*________________ alloc smaller then memory & not allined  __________________*/

	block_ptr[i] = VSAAlloc(vsa, block_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = (void *)((char *)memory_vsa 
		                                                 + BYTES_IN_VSA_STRUCT);
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr[i];
		
	TestResultComper(results, expected_results, test_name_1 , __LINE__);

	++i;


/*_____________ alloc smaller then memory but bigger then left _______________*/


	block_ptr[i] = VSAAlloc(vsa, block_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = NULL;
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr[i];
		
	TestResultComper(results, expected_results, test_name_2 , __LINE__);

	++i;


/*_________________ alloc smaller then left &  but allined ___________________*/


	block_ptr[i] = VSAAlloc(vsa, block_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = (void *)((char *)memory_vsa 
		                                 + block_size_allined[0]
		                                 + 2 * BYTES_IN_VSA_STRUCT);
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr[i];

		
	TestResultComper(results, expected_results, test_name_3 , __LINE__);

	++i;


/*______________________ alloc after free & difregment _______________________*/

	
	VSAFree(block_ptr[2]);

	block_ptr[i] = VSAAlloc(vsa, block_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = (void *)((char *)memory_vsa 
		                                 + block_size_allined[0]
		                                 + 2 * BYTES_IN_VSA_STRUCT);
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr[i];
		
	TestResultComper(results, expected_results, test_name_4 , __LINE__);

	++i;


/*____________________ alloc after free all & difregment _____________________*/

	
	VSAFree(block_ptr[0]);
	VSAFree(block_ptr[3]);

	block_ptr[i] = VSAAlloc(vsa, block_size[i]);

	expected_results.largest_free_block = expected_largest_free_block[i];
	expected_results.alloc_ptr = (void *)((char *)memory_vsa 
		                                                 + BYTES_IN_VSA_STRUCT);
		
	results.largest_free_block = VSALargestFreeBlock(vsa);
	results.alloc_ptr = block_ptr[i];
		
	TestResultComper(results, expected_results, test_name_5 , __LINE__);

	FREE(memory_vsa)
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 		
	const char *test_object= "Variable Size Allocator Test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.largest_free_block  != expected_results.largest_free_block)
	{
		printf(RED);
		printf("\nThe largest_free_block test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}

	if(results.alloc_ptr != expected_results.alloc_ptr)
	{
		printf(RED);
		printf("\nThe alloc_ptr test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}

	if(test_is_successful_flag == ILRD_TRUE)
	{
		printf(GREEN);
		printf("\n %s %s passed !\n", test_name ,test_object);
		printf(WHITE);
	} 
}


/*******************************************************************************
******************************* Begining Of Tests *****************************/

static void BeginingOfTests(const char *tests_name)
{
	printf(YELLOW);
	printf("\n______________________ %s _____________________\n\n", tests_name);
	printf(WHITE);
}

/*******************************************************************************
******************************* Begining Of Test ******************************/

static void BeginingOfTest(const char *tests_name)
{
	printf(WHITE);
	printf("\n\n > %s                   \n\n", tests_name);
	
}


/*******************************************************************************
********************************** End Of Tests *******************************/

static void EndOfTests(void)
{
	const char *end = "End Of Tests";

	printf(YELLOW);
	printf("\n_________________________ %s _______________________\n\n", end);
	printf(WHITE);
}

/*******************************************************************************
************************************ Remainder ********************************/

static void Remainder(void)
{
	const char *test_name = "remainder";

	BeginingOfTest(test_name);

	printf(RED);
	puts("\n DESTROY TEST - vlgrain is required \n");
	printf(WHITE);
}








