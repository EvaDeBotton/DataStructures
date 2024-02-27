/******************************************************************************
*	Utility Functions Implementations
*	authour : Nisan Ohana
*******************************************************************************/
#include <stdio.h> /* printf, puts */
#include <stdlib.h> /* rand , srand */
#include <time.h> /* time */
#include <string.h> /* memcmp*/

#include "utills.h"

/*****************************CmpResults***************************************/
void CmpResults(int result, int expected_result, 
      unsigned int *error_counter,size_t num_of_iteration,const char *test_name)
{
	if (result != expected_result)
	{
		printf(RED);
		printf("error in %s, mistake occured in itration number %ld\n"
		                                         , test_name, num_of_iteration);
		printf(WHITE);
		*error_counter += 1;
		
	}
}
/*****************************SuccessCheck*************************************/
void SuccessCheck(unsigned int error_counter, const char *test_name)
{
	if (NO_ERROR == error_counter)
	{
		printf(GREEN);
		printf("Success! the %s test run successful\n", test_name);
		puts("---------------------------------------------------------------");
		printf(WHITE);
	}
}
/*****************************CreateRandomNumbers*******************************/
void CreateRandomNumbers(int *num_arr, size_t array_size)
{
	size_t i = 0;
	srand(time(0));

	for (i = 0; i < array_size; i++)
	{
		num_arr[i] = (rand() % 1000);
	}
}
/*****************************CreateRandomNumbers*******************************/
void CmpPtr(void *expected_result, void *result, size_t num_bytes_to_cmp,
 unsigned int *error_counter, size_t num_of_iteration, const char *test_name)
{
	if (0 != memcmp(expected_result, result, num_bytes_to_cmp))
	{
		printf(RED);
		printf("error in %s, mistake occured in itration number %ld\n"
		                                         , test_name, num_of_iteration);
		printf(WHITE);
		*error_counter += 1;
	}
}	
/*****************************CreateRandomNumbers*******************************/
void PrintTestName(const char *test_name)
{
	printf("This is %s \n", test_name);
}