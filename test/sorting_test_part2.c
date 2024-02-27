/******************************************************************************
* Project: SORTING ALGORITHMS                                                 *
*                                                                             *
* wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by : Philip - comparsion sort                                       *
*              Nisan  - non comparsion sort                                   *
*                                                                             *
*                                                                             *
******************************************************************************/  

#include <stdio.h>  /* printf                      */
#include <stddef.h> /* size_t                      */
#include <stdlib.h> /* malloc, free, rand, qsort   */
#include <assert.h> /* assert                      */
#include <time.h>   /* clock, CLOCKS_PER_SEC       */

#include "sorting.h"

enum bool_vale
{
	ILRD_FALSE = 0,
	ILRD_TRUE = 1
};

enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 

/* colors for debugging */
#define RED			                    "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                         "\033[0;37m"
 
#define NUM_OF_ELEMENTS 5555
#define NUM_OF_REPEATS   100

#define FREE(ptr) {free(ptr); ptr = NULL;};
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);

typedef struct results
{  
	int *arr;
	size_t arr_size;
	double run_time;
	
} results_ty;

typedef struct results_u
{  
	unsigned int *arr;
	size_t arr_size;
	double run_time;
	
} results_ui_ty;

typedef int  (*merge_sorting_ty) (int *unsorted_arr, size_t arr_size);
typedef void (*quick_sortintg_ty)(void *base, size_t num_of_elements, 
	                                size_t elem_size, cmp_func_ty cmp_func);


/******************************* test functions *******************************/

void MergeSortTests(merge_sorting_ty func, const char *sorting_algorithm);

void SortedArrayMergeSortTest(merge_sorting_ty func);
void UnsortedArrayMergeSortTest(merge_sorting_ty func);
void RandomArrayMergeSortTests(merge_sorting_ty func);

void QuickSortTests(quick_sortintg_ty func, const char *sorting_algorithm);

void SortedArrayQuickSortTest(quick_sortintg_ty func);
void UnsortedArrayQuickSortTest(quick_sortintg_ty func);
void RandomArrayQuickSortTests(quick_sortintg_ty func);


/******************************* helper functions *****************************/

static void BeginingOfTests(const char *test_name);
static void BeginingOfTest( const char *tests_name);
static void EndOfTests(void);
static void Remainder(void);
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void BenchmarComper(double expected_results_average_time,
	                                                double results_average_time);
static int CmpFunc(const void *num1, const void *num2);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{    

	const char *tests_name = "Sorting Algorithms Test";

	const char *comparsion_algorithm = "MERGE SORT ";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	MergeSortTests(MergeSort,comparsion_algorithm);
	QuickSortTests(QuickSort,comparsion_algorithm);

	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
****************************** Sorting Test Merge *****************************/

void MergeSortTests(merge_sorting_ty func, const char *sorting_algorithm)
{

	BeginingOfTest(sorting_algorithm);

	SortedArrayMergeSortTest(func);
	UnsortedArrayMergeSortTest(func);
	RandomArrayMergeSortTests(func);

}

/*******************************************************************************
****************************** Sorting Test Merge *****************************/

void QuickSortTests(quick_sortintg_ty func, const char *sorting_algorithm)
{

	BeginingOfTest(sorting_algorithm);

	SortedArrayQuickSortTest(func);
	UnsortedArrayQuickSortTest(func);
	RandomArrayQuickSortTests(func);

}

/*******************************************************************************
************************* Sorted Array Merge Sort Test ************************/

void SortedArrayMergeSortTest(merge_sorting_ty func)
{
	const char *sorted_test = "  sorted array ";

	int *sorted_arr   = NULL;
	int *qsort_sorted_arr   = NULL;

	results_ty expected_results;
	results_ty results;

	int i = 0;
	size_t start_t;
	size_t end_t;

	sorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == sorted_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_sorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_sorted_arr)
   {
   	FREE(sorted_arr)
      puts("the system fail to allocate space");
      return;
   }

	/* Initialize sorted arrays */
	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		sorted_arr[i] = i + 1;
		qsort_sorted_arr[i] = i + 1; 
	}

	start_t = clock();
	func(sorted_arr ,NUM_OF_ELEMENTS);
	end_t = clock();
	
	results.arr = sorted_arr;
	results.arr_size = NUM_OF_ELEMENTS;
	results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	start_t = clock();
	qsort(qsort_sorted_arr ,NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();

	expected_results.arr = qsort_sorted_arr;
	expected_results.arr_size = NUM_OF_ELEMENTS;
	expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	TestResultComper(results, expected_results, sorted_test, __LINE__);

	FREE(sorted_arr)
	FREE(qsort_sorted_arr)
}

/*******************************************************************************
************************** Unsorted Array Merge Sort Test *********************/

void UnsortedArrayMergeSortTest(merge_sorting_ty func)
{
	const char *unsorted_test = " unsorted array";
	
	int *unsorted_arr = NULL;
	int *qsort_unsorted_arr = NULL;

	results_ty expected_results;
	results_ty results; 

	int i = 0;
	size_t start_t;
	size_t end_t;

	unsorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == unsorted_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_unsorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_unsorted_arr)
   {
   	FREE(unsorted_arr)
      puts("the system fail to allocate space");
      return;
   }

	/* Initialize unsorted arrays */
	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		unsorted_arr[i] = NUM_OF_ELEMENTS - i;
		qsort_unsorted_arr[i] = NUM_OF_ELEMENTS - i; 
	}

	start_t = clock();
	func(unsorted_arr ,NUM_OF_ELEMENTS);
	end_t = clock();
	
	results.arr = unsorted_arr;
	results.arr_size = NUM_OF_ELEMENTS;
	results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	start_t = clock();
	qsort(qsort_unsorted_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();

	expected_results.arr = qsort_unsorted_arr;
	expected_results.arr_size = NUM_OF_ELEMENTS;
	expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	TestResultComper(results, expected_results, unsorted_test, __LINE__);

	FREE(unsorted_arr)
	FREE(qsort_unsorted_arr)

}

/*******************************************************************************
************************* Random Array Merge Sort Tests ***********************/

void RandomArrayMergeSortTests(merge_sorting_ty func)
{
	const char *random_test = "  random array ";
	
	int *random_arr  = NULL; 
	int *qsort_random_arr   = NULL;
	
	results_ty expected_results;
	results_ty results;

	int i = 0;
	int j = 0;
	int number_of_repeat = NUM_OF_REPEATS;
	size_t start_t;
	size_t end_t;

	double results_average_time = 0;
	double expected_results_average_time = 0;

	random_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == random_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_random_arr = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_random_arr)
   {
   	FREE(random_arr)
      puts("the system fail to allocate space");
      return;
   }

	for(j = 0; j < number_of_repeat; j++)
	{
		/* Initialize random arrays */
		for(i = 0; i < NUM_OF_ELEMENTS; i++)
		{
			random_arr[i] = rand() % 500;
			qsort_random_arr[i] = random_arr[i]; 
		}

		start_t = clock();
		func((void *)random_arr ,NUM_OF_ELEMENTS);
		end_t = clock();
	
		results.arr = random_arr;
		results.arr_size = NUM_OF_ELEMENTS;
		results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

		start_t = clock();
		qsort(qsort_random_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
		end_t = clock();

		expected_results.arr = qsort_random_arr;
		expected_results.arr_size = NUM_OF_ELEMENTS;
		expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

		results_average_time += results.run_time;
		expected_results_average_time += expected_results.run_time;
	}

	expected_results_average_time /= ((double)number_of_repeat);
	results_average_time /= ((double)number_of_repeat); 

	results.run_time = results_average_time;
	expected_results.run_time = expected_results_average_time;

	TestResultComper(results, expected_results, random_test, __LINE__);

	BenchmarComper(expected_results_average_time,results_average_time);

	FREE(random_arr)
	FREE(qsort_random_arr)
}


/*******************************************************************************
************************* Sorted Array Quick Sort Test ************************/

void SortedArrayQuickSortTest(quick_sortintg_ty func)
{
	const char *sorted_test = "  sorted array ";

	int *sorted_arr   = NULL;
	int *qsort_sorted_arr   = NULL;

	results_ty expected_results;
	results_ty results;

	int i = 0;
	size_t start_t;
	size_t end_t;

	sorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == sorted_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_sorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_sorted_arr)
   {
   	FREE(sorted_arr)
      puts("the system fail to allocate space");
      return;
   }

	/* Initialize sorted arrays */
	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		sorted_arr[i] = i + 1;
		qsort_sorted_arr[i] = i + 1; 
	}

	start_t = clock();
	func(sorted_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();
	
	results.arr = sorted_arr;
	results.arr_size = NUM_OF_ELEMENTS;
	results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	start_t = clock();
	qsort(qsort_sorted_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();

	expected_results.arr = qsort_sorted_arr;
	expected_results.arr_size = NUM_OF_ELEMENTS;
	expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	TestResultComper(results, expected_results, sorted_test, __LINE__);

	FREE(sorted_arr)
	FREE(qsort_sorted_arr)
}

/*******************************************************************************
************************* Unsorted Array Quick Sort Test **********************/

void UnsortedArrayQuickSortTest(quick_sortintg_ty func)
{
	const char *unsorted_test = " unsorted array";
	
	int *unsorted_arr = NULL;
	int *qsort_unsorted_arr = NULL;

	results_ty expected_results;
	results_ty results; 

	int i = 0;
	size_t start_t;
	size_t end_t;

	unsorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == unsorted_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_unsorted_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_unsorted_arr)
   {
   	FREE(unsorted_arr)
      puts("the system fail to allocate space");
      return;
   }

	/* Initialize unsorted arrays */
	for(i = 0; i < NUM_OF_ELEMENTS; i++)
	{
		unsorted_arr[i] = NUM_OF_ELEMENTS - i;
		qsort_unsorted_arr[i] = NUM_OF_ELEMENTS - i; 
	}

	start_t = clock();
	func(unsorted_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();
	
	results.arr = unsorted_arr;
	results.arr_size = NUM_OF_ELEMENTS;
	results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	start_t = clock();
	qsort(qsort_unsorted_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
	end_t = clock();

	expected_results.arr = qsort_unsorted_arr;
	expected_results.arr_size = NUM_OF_ELEMENTS;
	expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

	TestResultComper(results, expected_results, unsorted_test, __LINE__);

	FREE(unsorted_arr)
	FREE(qsort_unsorted_arr)

}

/*******************************************************************************
************************** Random Array Quick Sort Tests **********************/

void RandomArrayQuickSortTests(quick_sortintg_ty func)
{
	const char *random_test = "  random array ";
	
	int *random_arr  = NULL; 
	int *qsort_random_arr   = NULL;
	
	results_ty expected_results;
	results_ty results;

	int i = 0;
	int j = 0;
	int number_of_repeat = NUM_OF_REPEATS;
	size_t start_t;
	size_t end_t;

	double results_average_time = 0;
	double expected_results_average_time = 0;

	random_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == random_arr)
   {
      puts("the system fail to allocate space");
      return;
   }

	qsort_random_arr   = MALLOC(NUM_OF_ELEMENTS, int)

	if(NULL == qsort_random_arr)
   {
   	FREE(random_arr)
      puts("the system fail to allocate space");
      return;
   }

	for(j = 0; j < number_of_repeat; j++)
	{
		/* Initialize random arrays */
		for(i = 0; i < NUM_OF_ELEMENTS; i++)
		{
			random_arr[i] = rand() % 500;
			qsort_random_arr[i] = random_arr[i]; 
		}

		start_t = clock();
		func(random_arr, NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
		end_t = clock();
	
		results.arr = random_arr;
		results.arr_size = NUM_OF_ELEMENTS;
		results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

		start_t = clock();
		qsort(qsort_random_arr , NUM_OF_ELEMENTS, sizeof(int), CmpFunc);
		end_t = clock();

		expected_results.arr = qsort_random_arr;
		expected_results.arr_size = NUM_OF_ELEMENTS;
		expected_results.run_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

		results_average_time += results.run_time;
		expected_results_average_time += expected_results.run_time;
	}

	expected_results_average_time /= ((double)number_of_repeat);
	results_average_time /= ((double)number_of_repeat); 

	results.run_time = results_average_time;
	expected_results.run_time = expected_results_average_time;

	TestResultComper(results, expected_results, random_test, __LINE__);

	BenchmarComper(expected_results_average_time,results_average_time);

	FREE(random_arr)
	FREE(qsort_random_arr)
}

/*******************************************************************************
******************************* Benchmar Comper *******************************/

static void BenchmarComper(double expected_results_average_time,
	                                                 double results_average_time)
{
	if (results_average_time < expected_results_average_time)
	{
		printf(YELLOW);
		printf("\n\nBENCHMARK  :  This algorithm is FASTER %f times then qsort\n \
             in sorting %d elements\n\n ",
	        expected_results_average_time/results_average_time, NUM_OF_ELEMENTS);
	}

	if (results_average_time > expected_results_average_time)
	{
		printf(YELLOW);
		printf("\n\nBENCHMARK  :  This algorithm is SLOWER %f times then qsort\n \
             in sorting %d elements\n\n ",
	        results_average_time/expected_results_average_time, NUM_OF_ELEMENTS);
	}

		printf(WHITE);
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line)
{ 
	int test_is_successful_flag = ILRD_TRUE;

	size_t i = 0;

	printf(GREEN);
	printf("\n_______________________ %s ______________________\n",test_name);


	for(i = 0; i < results.arr_size; i++)
	{
		if(results.arr[i] != expected_results.arr[i])
		{
			printf(RED);
			printf("\nThe sorting fail at iteration %ld\n\n",i);
			test_is_successful_flag = ILRD_FALSE;
			printf(WHITE);
			return;
		}
	}

	if(test_is_successful_flag == ILRD_TRUE)
	{
		printf(GREEN);
		printf("\n sorting status for %s :   SUCCESS\n",test_name);
		printf("\n sorting time of %s for sorting algorithm:  %f\n",
	                                                test_name,results.run_time);
		printf("\n sorting time of %s for Qsort   algorithm:  %f\n",
	                                      test_name, expected_results.run_time);
		printf(WHITE);
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
}

/*******************************************************************************
************************************ Remainder ********************************/

static void Remainder(void)
{
	printf(RED);
	puts("\n\n\nDESTROY TEST - vlgrain is required \n");
}

/*******************************************************************************
*********************************** Cmper Func ********************************/

static int CmpFunc (const void *num1, const void *num2)
{
   return ( *(int *)num1 - *(int *)num2 );
}








