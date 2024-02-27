#ifndef __UTILLS_H__
#define __UTILLS_H__

#include <stdio.h> /* printf, puts */
#include <stdlib.h> /* rand , srand */
#include <time.h> /* time */
#include <string.h> /* memcmp*/


#ifndef NDEBUG
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x) 
#endif 

/* ifndef NDEBUG */

#define FREE(memory) {free(memory); memory = NULL;}

/*********************************Macros****************************************/
/* colors for debugging */
#define BLACK			"\033[0;30m"
#define RED				"\033[0;31m"
#define GREEN 			"\033[0;32m"
#define YELLOW			"\033[0;33m"
#define BLUE			"\033[0;34m"
#define PURPLE			"\033[0;35m"
#define CYAN			"\033[0;36m"
#define WHITE 			"\033[0;37m"
/* for successcheck */
#define NO_ERROR 0 

/* array utilitis */
#define ARR_SIZE(arr) sizeof((arr))/sizeof((arr[0]))

/*********************************EnumDeclarations*****************************/
enum ilrd_status
{
	ILRD_SUCCESS = 0, 
	ILRD_FAILURE = 1
};

enum bool_vale
{
	ILRD_FALSE = 0,
	ILRD_TRUE = 1
};

void CmpResults(int result, int expected_result, 
      unsigned int *error_counter,size_t num_of_iteration,const char *test_name);

void SuccessCheck(unsigned int error_counter, const char *test_name);


void CreateRandomNumbers(int *num_arr, size_t array_size);

void CmpPtr(void *expected_result, void *result, size_t num_bytes_to_cmp,
 unsigned int *error_counter, size_t num_of_iteration, const char *test_name);



#endif /* __UTILLS_H__ */
