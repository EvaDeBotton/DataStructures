/******************************************************************************
* Calculator                                                                *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                                 *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
#include "ht.h"
#include "dictionary.h"

/* colors for debugging */
#define RED			                     "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                            "\033[0;37m"

#define NOT_YET                          0
#define SIZE_ZERO                        0
#define NUM_OF_ELEMENTS                 10

enum is_empty 
{
	NOT_EMPTY = 0, 
	EMPTY  = 1 
};

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum compare
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

enum return_status 
{
   ILRD_SUCSSES = 0, 
   ILRD_FAILURE = 1 
};

typedef struct results
{  
	size_t size;
	int is_empty;
	void *find;
	double sd;
	double load_factor;
	
} results_ty;

typedef struct user_data
{  
	void *key;
	void *data;
	
} user_data_ty;

/* test functions */
void TestHTCreate(void);
void TestHTInsert(void);
void TestHTSpell(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/* user function */

/* hash_func_ty return size_t type after manipulation on the key*/
static size_t SimpleHash(const void *key);

/* return the key of data item we get, so we know where to insert data */
static void* GetKey(const void *data);

/* this hash table match function */
static int HashMatch(const void *data, void *param);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Calculator Tests";

	BeginingOfTests(tests_name);

	/* The main test functions*/ 
	TestHTCreate();
	TestHTInsert();
	TestHTSpell();

	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
*************************** Test Hash Table Create ****************************/

void TestHTCreate(void)
{ 
	const char *test_name = "CREATE"; 

	results_ty expected_results;
	results_ty results;

	size_t capacity = 10; 

	ht_ty *hash_table = HTCreate(capacity, SimpleHash, GetKey, HashMatch);

	expected_results.size = SIZE_ZERO;
	expected_results.is_empty = EMPTY;
	expected_results.find = NOT_YET;
	expected_results.sd = NOT_YET;
	expected_results.load_factor = NOT_YET;

	results.size = HTSize(hash_table);
	results.is_empty = HTIsEmpty(hash_table);
	results.find = NOT_YET;
	results.sd = NOT_YET;
	results.load_factor = NOT_YET;

	TestResultComper(results, expected_results, test_name, __LINE__);

	HTDestroy(hash_table); 
}

/*******************************************************************************
*************************** Test Hash Table Insert ****************************/

void TestHTInsert(void)
{ 
	const char *test_name = "INSERT";

	results_ty expected_results;
	results_ty results;

	size_t capacity = 10;

	int i = 0;
	int status = 0;

	ht_ty *hash_table = HTCreate(capacity, SimpleHash, GetKey, HashMatch);

	double arr_of_data[NUM_OF_ELEMENTS] = {1.1, 1.2, 3.5, 7, 8, 9, 9.2, 2.2, 1.3, 5.4};
	int arr_of_keys[NUM_OF_ELEMENTS];

	user_data_ty numbers[NUM_OF_ELEMENTS];

	for(i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		arr_of_keys[i]   = (int )arr_of_data[i];
		numbers[i].key  = (void *)&arr_of_keys[i];
		numbers[i].data = (void *)&arr_of_data[i];
	} 

	for(i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		status = HTInsert(hash_table ,&numbers[i]);

		if (ILRD_FAILURE == status)
		{
			printf("allocate fail");
			return;
		}
	}

	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.is_empty = NOT_EMPTY;
	expected_results.find = NOT_YET;
	expected_results.sd = NOT_YET;
	expected_results.load_factor = NOT_YET;

	results.size = HTSize(hash_table);
	results.is_empty = HTIsEmpty(hash_table);
	results.find = NOT_YET;
	results.sd = NOT_YET;
	results.load_factor = NOT_YET;

	TestResultComper(results, expected_results, test_name, __LINE__);

	HTDestroy(hash_table);
}

/*******************************************************************************
*************************** Test Hash Table Insert ****************************/

void TestHTSpell(void)
{ 
	const char *test_name = "SPELL";

	results_ty expected_results;
	results_ty results;

	size_t capacity = 26;

	int i = 0;
	int status = 0;

	ht_ty *hash_table = HTCreate(capacity, SimpleHash, GetKey, HashMatch);

	double arr_of_data[NUM_OF_ELEMENTS] = {1.1, 1.2, 3.5, 7, 8, 9, 9.2, 2.2, 1.3, 5.4};
	int arr_of_keys[NUM_OF_ELEMENTS];

	user_data_ty numbers[NUM_OF_ELEMENTS];

	for(i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		arr_of_keys[i]   = (int )arr_of_data[i];
		numbers[i].key  = (void *)&arr_of_keys[i];
		numbers[i].data = (void *)&arr_of_data[i];
	} 

	for(i = 0; i < NUM_OF_ELEMENTS ; i++)
	{
		status = HTInsert(hash_table ,&numbers[i]);

		if (ILRD_FAILURE == status)
		{
			printf("allocate fail");
			return;
		}
	}

	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.is_empty = NOT_EMPTY;
	expected_results.find = NOT_YET;
	expected_results.sd = NOT_YET;
	expected_results.load_factor = NOT_YET;

	results.size = HTSize(hash_table);
	results.is_empty = HTIsEmpty(hash_table);
	results.find = NOT_YET;
	results.sd = NOT_YET;
	results.load_factor = NOT_YET;

	TestResultComper(results, expected_results, test_name, __LINE__);

	HTDestroy(hash_table);
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "Hash-Table Test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.size != expected_results.size)
	{
		printf(RED);
		printf("\nThe size test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}
	if(results.is_empty != expected_results.is_empty)
	{
		printf(RED);
		printf("\nThe is_empty test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}
	if(results.find != expected_results.find)
	{
		printf(RED);
		printf("\nThe find test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}
	if(results.sd != expected_results.sd)
	{
		printf(RED);
		printf("\nThe sd test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}
	if(results.load_factor != expected_results.load_factor)
	{
		printf(RED);
		printf("\nThe load_factor test fail for %s %s at line %lu\n",
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
	printf(RED);
	puts("\n DESTROY TEST - vlgrain is required \n");
	printf(WHITE);
}

/*******************************************************************************
********************************* User Functions ******************************/

/* hash_func_ty return size_t type after manipulation on the key*/
static size_t SimpleHash(const void *key)
{
	size_t index = (size_t)(*(int *)key);

	return(index);
}

/* return the key of data item we get, so we know where to insert data */
static void* GetKey(const void *data)
{
	user_data_ty *user_data = (user_data_ty *)data;

	return(user_data->key);
}

/* this hash table match function */
static int HashMatch(const void *data, void *param)
{
	if(*(int*)data > *(int*)param)
	{
		return BIGGER;
	}
	else if(*(double*)data < *(double*)param)
	{
		return SMALLER;
	}

	return EQUAL;
}

static int AddOne(void *data, void *param)
{

}



