#ifndef __UTILS_TEST_H__
#define __UTILS_TEST_H__
 
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf, puts */
#include <stdlib.h> /* rand , srand */
#include <time.h> /* time */
#include <string.h> /* memcmp*/
#include <stdarg.h>    /*variable number of arguments ...: va_list*/

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
enum allc_status
{
	ALLOC_FAIL = -1
};

enum is_empty
{
	NOT_EMPTY = 0,
	EMPTY = 1
};

enum is_equal
{
	NOT_EQUAL = 0, 
	IS_EQUAL = 1
};

enum is_match
{
	NO_MATCH = 0,
	MATCH = 1
};
/******************************************************************************/
#ifndef NDEBUG
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x) 
#endif 

#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define UNUSED_VAR(var) ((void)var)
#define IDX_ARR_SIZE 100

#define XSTR(s) STR(s)
#define STR(s) #s

#define ESC			\033

#define BLACK 		ESC[0;30m
#define RED 		ESC[0;31m
#define GREEN		ESC[0;32m
#define YELLOW  	ESC[0;33m
#define BLUE  		ESC[0;34m
#define PURPLE  	ESC[0;35m
#define CYAN 		ESC[0;36m
#define WHITE		ESC[0;37m

#define BLACK_BOLD 	ESC[1;30m
#define RED_BOLD 	ESC[1;31m
#define GREEN_BOLD	ESC[1;32m
#define YELLOW_BOLD ESC[1;33m
#define BLUE_BOLD	ESC[1;34m
#define PURPLE_BOLD ESC[1;35m
#define CYAN_BOLD 	ESC[1;36m
#define WHITE_BOLD	ESC[1;37m

#define RESET		ESC[0m
/* for successcheck */
#define NO_ERROR 0 

/* array utilities */
#define ARR_SIZE(arr) sizeof((arr))/sizeof((arr[0]))


void Black(void);
void Red(void);
void Green(void);
void Yellow(void);
void Blue(void);
void Purple(void);
void Cyan(void);
void White(void);

void BlackBold(void);
void RedBold(void);
void GreenBold(void);
void YellowBold(void);
void BlueBold(void);
void PurpleBold(void);
void CyanBold(void);
void WhiteBold(void);
void Reset(void);


/******************************************************************************/
/* Compares expected to result and prints informative message if not equal */
void TestResExpected (size_t expected, size_t res, const char *test_name,
					 const char *file_name, size_t line_num);
					 
/* Prints a message that given test has finished */
void PrintTestFinished(const char *test_name);

/* Expectation is that ptr is not null. If it is, print a fail message */ 
void TestNotNull(void *ptr, const char *test_name,
				const char *file_name, size_t line);
				
/* Expectation is that ptr is null. If it is not, print a fail message */
void TestNull(void *ptr, const char *test_name,
				const char *file_name, size_t line);

/* Initializes an index array int the given dest. The func assigns dest[i] = i
	size must be > 0 */
unsigned int *InitIdxArr(unsigned int *dest, unsigned int size);
				
/******************************************************************************/
			
void CmpResults(int result, int expected_result, 
      unsigned int *error_counter,size_t num_of_iteration,const char *test_name);

void SuccessCheck(unsigned int error_counter, const char *test_name);


void CreateRandomNumbers(int *num_arr, size_t array_size);


void CmpPtr(void *expected_result, void *result, size_t num_bytes_to_cmp,
 unsigned int *error_counter, size_t num_of_iteration, const char *test_name);
 
/******************************************************************************/
/*return -1 on fail. 0 on success*/
int CheckAlloc(int num, ...);

#endif /* __TEST_UTILS_H__ */
