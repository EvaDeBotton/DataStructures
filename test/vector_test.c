/******************************************************************************
* Vector_test.c                                                               *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Liel                                                             *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /*size_t */
#include "../include/Vector.h"

/* colors for debugging */
#define BLACK			"\033[0;30m"
#define RED			    "\033[0;31m"
#define GREEN 			"\033[0;32m"
#define YELLOW			"\033[0;33m"
#define BLUE			"\033[0;34m"
#define PURPLE			"\033[0;35m"
#define CYAN			"\033[0;36m"
#define WHITE 			"\033[0;37m"

#define GF                                          2                          
#define GF_SQUARED                          (GF)*(GF)
#define CAPACITY                                   20
#define ONE                                         1                    
#define SIZE_ZERO                                   0  
#define SUCSSES                                     0
#define FAILURE                                     1

typedef struct results
{  
	size_t capacity;
	size_t init_capacity;
	size_t size;
	void *get_val;
	
} results_ty;

struct vector
{
	void **array;   
	size_t size;
	size_t capacity;
	size_t init_capacity;
};

/* actuall tests */
void InitializeVectorTests(void);
void FullVectorTests(void);
void PushVectorTests(void);
void PopsAtInitialCapacityIntervalVectorTests(void);
void PopsAfterInitialCapacityIntervalVectorTests(void);
void ShrinkToFitVectorTests(void);
void ReserveForBiggerCapacityTests(void);
void ReserveForSmallerCapacityTests(void);
void EmptyVectorTests(void);
void InitCapacityZeroTests(void);
void EndOfTests(void);

/* helper */
static void VectorMultiPush(vector_ty *vector, size_t num_of_pushs, 
                                                                 void *element);
static void VectorMultiPops(vector_ty *vector, size_t num_of_pops);
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	InitializeVectorTests();
	FullVectorTests();
	PushVectorTests();
	PopsAtInitialCapacityIntervalVectorTests();
	PopsAfterInitialCapacityIntervalVectorTests();
	ShrinkToFitVectorTests();
	ReserveForBiggerCapacityTests();
	ReserveForSmallerCapacityTests();
	EmptyVectorTests();
	InitCapacityZeroTests();
	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Initialize Vector Test ***************************/

/*	test case: checking all the struct parameter after initializing
*/
void InitializeVectorTests(void)
{ 
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Initialize Vector";
	results_ty expected_results;
	results_ty results;
	
	expected_results.capacity = CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.get_val = NULL;

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = NULL;
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************************* Full Vector Test ******************************/

/*	test case: checking all the struct parameter after fill all the 
	           init_capacity elements in the array.
*/
void FullVectorTests(void)
{ 
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Full Vector";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = CAPACITY;
	expected_results.get_val = ptr_to_test;
	
	VectorMultiPush(vector, CAPACITY, ptr_to_test); 

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector, CAPACITY - ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
******************************* Push Vector Test ******************************/

/*	test case: checking all the struct parameter after pushing  
               CAPACITY * GF + ONE times.
*/
void PushVectorTests(void)
{ 
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Push Vector";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = GF_SQUARED * CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = CAPACITY * GF + ONE;
	expected_results.get_val = ptr_to_test;
	
	VectorMultiPush(vector,(CAPACITY * GF + ONE), ptr_to_test); 
	VectorSetVal(vector,(CAPACITY * GF_SQUARED - ONE), ptr_to_test);

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector,CAPACITY * GF_SQUARED - ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
} 

/*******************************************************************************
*************** Pops At Initial Capacity Interval Vector Test *****************/

/*	test case: checking all the struct parameter after poping while capacity/2 
               is smaller then init_capacity.
*/
void PopsAtInitialCapacityIntervalVectorTests(void)
{ 
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Pops in the initial capacity interval Vector";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.get_val = NULL;
	
	VectorMultiPush(vector, CAPACITY, ptr_to_test);
	/* poping all the element should not change the capacity */ 
	VectorMultiPops(vector, CAPACITY);

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = NULL;
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
************* Pops After Initial Capacity Interval Vector Test ****************/

/*	test case: checking all the struct parameter after poping while capacity/2 
               is bigger then init_capacity.
*/
void PopsAfterInitialCapacityIntervalVectorTests(void)
{ 
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Pops After the initial capacity interval Vector";
	results_ty expected_results;
	results_ty results;
	size_t num_of_push = GF_SQUARED * CAPACITY + ONE;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = GF * CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.get_val = ptr_to_test;
	
	/* The new capacity is GF^3 * CAPACITY */
	VectorMultiPush(vector, num_of_push, ptr_to_test);
	/* In order that the capacity shrink, the size need to by 1/4 from 
	   the capacity  */
	VectorMultiPops(vector, num_of_push);
	
	VectorSetVal(vector, GF_SQUARED * CAPACITY - ONE, ptr_to_test);

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector,GF_SQUARED * CAPACITY - ONE);

	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
************************** Shrink To Fit Vector Test **************************/

/*	test case: checking all the struct parameter after shrink to fit.
*/
void ShrinkToFitVectorTests(void)
{
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Shrink To Fit Vector";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY / GF;
	expected_results.init_capacity = CAPACITY / GF;
	expected_results.size = CAPACITY / GF;
	expected_results.get_val = ptr_to_test;
	
	/* push only CAPACITY / GF elements */
	VectorMultiPush(vector, (CAPACITY / GF), ptr_to_test);
	VectorShrinkToFit(vector); 

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector,(CAPACITY / GF)- ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
********************** Reserve For Bigger Capacity Test ***********************/

/*	test case: checking all the struct parameter after reserve with new capacity
	           bigger then the current capacity.       
*/
void ReserveForBiggerCapacityTests(void)
{
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Reserve For Bigger Capacity";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY * GF;
	expected_results.init_capacity = CAPACITY * GF;
	expected_results.size = CAPACITY;
	expected_results.get_val = ptr_to_test;
	
	VectorMultiPush(vector, CAPACITY, ptr_to_test);
	VectorReserve(vector, CAPACITY * GF);
	VectorSetVal(vector, CAPACITY * GF - ONE, ptr_to_test); 

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector, CAPACITY * GF - ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
********************** Reserve For Smaller Capacity Test ***********************/

/*	test case: checking all the struct parameter after reserve with new capacity
	           smaller then the current capacity.       
*/
void ReserveForSmallerCapacityTests(void)
{
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Reserve For Smaller Capacity";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = CAPACITY;
	expected_results.get_val = ptr_to_test;
	
	VectorMultiPush(vector, CAPACITY, ptr_to_test);
	VectorReserve(vector, CAPACITY / GF);
	VectorSetVal(vector, CAPACITY - ONE, ptr_to_test); 

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector, CAPACITY - ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
****************************** Empty Vector Tests *****************************/

/*	test case: checking all the struct parameter after poping all the element.      
*/
void EmptyVectorTests(void)
{
	vector_ty *vector = VectorCreate(CAPACITY);
	const char *test_name = "Empty Vector";
	results_ty expected_results;
	results_ty results;
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = CAPACITY * GF;
	expected_results.init_capacity = CAPACITY;
	expected_results.size = SIZE_ZERO;
	expected_results.get_val = NULL;
	
	VectorMultiPush(vector, CAPACITY + ONE, ptr_to_test);
	VectorMultiPops(vector, CAPACITY + ONE);

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = NULL;
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
********************** Init Capacity Zero Vector Tests ************************/

/*	test case: checking all the struct parameter after initializing whit zero 
               capacity, and pushing n elements.      
*/
void InitCapacityZeroTests(void)
{
	vector_ty *vector = VectorCreate(SIZE_ZERO);
	const char *test_name = "Init Capacity Zero Vector";
	results_ty expected_results;
	results_ty results;
	size_t num_to_push = 5; 
	
	size_t test = 5; 
	size_t *ptr_to_test = &test;
	
	expected_results.capacity = 8;
	expected_results.init_capacity = SIZE_ZERO;
	expected_results.size = num_to_push;
	expected_results.get_val = ptr_to_test;
	
	VectorMultiPush(vector, num_to_push, ptr_to_test);

	results.capacity = VectorCapacity(vector);
	results.init_capacity = vector->init_capacity;
	results.size = VectorSize(vector);
	results.get_val = VectorGetVal(vector, num_to_push - ONE);
	
	VectorDestroy(vector);
	
	TestResultComper(results, expected_results, test_name, __LINE__);	
}

/*******************************************************************************
******************************* Stack Multi Push ******************************/

static void VectorMultiPush(vector_ty *vector, size_t num_of_pushs, 
                                                                  void *element) 
{
	size_t i = 0; 
																	
	for(i = 0; i < num_of_pushs; i++)
	{
		VectorPushBack(vector, element);
	}
}

/*******************************************************************************
******************************* Stack Multi Pops ******************************/

static void VectorMultiPops(vector_ty *vector, size_t num_of_pops) 
{
	size_t i = 0;
	
	for(i = 0; i < num_of_pops; i++)
	{
		VectorPopBack(vector);
	}
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line)
{ 
	if(results.capacity != expected_results.capacity)
	{
		printf("\nThe Capacity test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.init_capacity != expected_results.init_capacity)
	{
		printf("\nThe init Capacity test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.size != expected_results.size)
	{
		printf("\nThe Size test fail for %s test at line %lu\n",
		                                                       test_name, line);
	}
	if(results.get_val != expected_results.get_val)
	{
		printf("\nThe get val test fail for %s test at line %lu\n",
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








