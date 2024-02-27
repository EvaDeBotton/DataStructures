/******************************************************************************
* Stack_test.c                                                                *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Nisan Ohana                                                         *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /*size_t */
#include <stdio.h> /*__LINE__ */
#include "../include/Stack.h"

#define CAPACITY               20
#define HALF_CAPACITY          10
#define EMPTY                   1
#define NOT_EMPTY               0
#define SIZE_ZERO               0


typedef struct results
{
	bool_ty is_empty;   
	size_t capacity;
	size_t size;
	void *peek;
	
} results_ty;

/* actuall tests */
void InitializeStackTests(void);
void FullStackTests(void);
void HalfFullStackTests(void);
void EmptyStackTests(void);
void EndOfTests(void);

/* helper */
static void StackMultiPush(stack_ty *stack, size_t num_of_pushs, void *element);
static void StackMultiPops(stack_ty *stack, size_t num_of_pops); 
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	InitializeStackTests();
	FullStackTests();
	HalfFullStackTests();
	EmptyStackTests();
	
	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
***************************** Initialize Stack Tests **************************/

void InitializeStackTests(void)
{ 
	stack_ty *stack = StackCreate(CAPACITY);
	const char *test_name = "Initialize Stack";
	results_ty expected_results;
	results_ty results;
	
	expected_results.is_empty = EMPTY;
	expected_results.capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.peek = NULL;

	results.is_empty = StackIsEmpty(stack);
	results.capacity = StackCapacity(stack);
	results.size = StackSize(stack);
	results.peek = NULL;
	
	StackDestroy(stack);
	
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************************** Full Stack Tests *****************************/

void FullStackTests(void)
{ 
	stack_ty *stack = StackCreate(CAPACITY);
	const char *test_name = "Full Stack";
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	results_ty expected_results;
	results_ty results;
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.capacity = CAPACITY;
	expected_results.size = CAPACITY;
	expected_results.peek = ptr_to_test;
	
	StackMultiPush(stack, CAPACITY, ptr_to_test);

	results.is_empty = StackIsEmpty(stack);
	results.capacity = StackCapacity(stack);
	results.size = StackSize(stack);
	results.peek = StackPeek(stack);
	
	StackDestroy(stack);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
***************************** Half Full Stack Tests ***************************/

void HalfFullStackTests(void)
{
	stack_ty *stack = StackCreate(CAPACITY);
	const char *test_name = "Half Full Stack";
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	results_ty expected_results;
	results_ty results;
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.capacity = CAPACITY;
	expected_results.size = HALF_CAPACITY;
	expected_results.peek = ptr_to_test;

	StackMultiPush(stack, CAPACITY, ptr_to_test);
	StackMultiPops(stack, HALF_CAPACITY);
	StackMultiPush(stack, HALF_CAPACITY, ptr_to_test);
	StackMultiPops(stack, HALF_CAPACITY);

	results.is_empty = StackIsEmpty(stack);
	results.capacity = StackCapacity(stack);
	results.size = StackSize(stack);
	results.peek = StackPeek(stack);
	
	StackDestroy(stack);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
******************************** Empty Stack Tests ****************************/

void EmptyStackTests(void)
{ 
	stack_ty *stack = StackCreate(CAPACITY);
	const char *test_name = "Empty Stack";
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.peek = NULL; 
	
	StackMultiPush(stack, CAPACITY, ptr_to_test);
	StackMultiPops(stack, CAPACITY);

	results.is_empty = StackIsEmpty(stack);
	results.capacity = StackCapacity(stack);
	results.size = StackSize(stack);
	results.peek = NULL;
	
	StackDestroy(stack);
	
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************************* Stack Multi Push ******************************/

static void StackMultiPush(stack_ty *stack, size_t num_of_pushs, void *element) 
{
	size_t i = 0; 
																	
	for(i = 0; i < num_of_pushs; i++)
	{
		StackPush(stack, element);
	}
}

/*******************************************************************************
******************************* Stack Multi Pops ******************************/

static void StackMultiPops(stack_ty *stack, size_t num_of_pops) 
{
	size_t i = 0;
	
	for(i = 0; i < num_of_pops; i++)
	{
		StackPop(stack);
	}
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line)
{ 
	if(results.is_empty != expected_results.is_empty)
	{
		printf("\nThe Empty Stack test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.capacity != expected_results.capacity)
	{
		printf("\nThe Capacity Stack test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.size != expected_results.size)
	{
		printf("\nThe Size Stack test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.peek != expected_results.peek)
	{
		printf("\nThe Peek Stack test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
}


/*******************************************************************************
********************************** End Of Tests *******************************/

void EndOfTests(void)
{
	printf("\n_________________________End Of Tests________________________\n");
}








