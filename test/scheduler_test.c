/******************************************************************************
* Sorted Scheduler_test.c                                                     *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Nasrat                                                              *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include "dlist.h"
#include <assert.h> /* assert */
#include "pqueue.h"
#include "utils.h"
#include "scheduler.h"
#include "uid.h"
#include <unistd.h> /* sleep */

#define FAILUER               1
#define SUCCESS               0 
#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS       3


typedef struct results
{  
	int is_empty;
	size_t size;		
    int remove_status;
    int status_of_execution;
	
} results_ty;

/* test functions */
void TestInitializeScheduler(void);
void TestAddToScheduler(void);
void TestRemoveScheduler(void);
void TestRunScheduler(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/* helper */
static void SchedulerMultiInsert(scheduler_ty *scheduler, size_t num_of_insert, 
                                                                    int *array);


/*
scheduler_func_ty is the type of the functions the user of the 
scheduler want to schedule for execution.

return value: status of execution.
				0 - success - completed
				1 - not completed
				2 - error 

argument: param - for user parameter/data.
*/

int PrintChar8time(void *ch);
int PrintChar4time(void *ch);

/******************************** Print Char **********************************/

int PrintChar8time(void *ch)
{
	static int repeat_number8 = 8;
	printf(" %c \n", *(char *)ch);

	fflush(stdout);

	--repeat_number8;

	return (!!repeat_number8);
}

/******************************** Print Char **********************************/

int PrintChar4time(void *ch)
{
	static int repeat_number4 = 4;
	printf(" %c \n", *(char *)ch);

	fflush(stdout);

	--repeat_number4;

	return (!!repeat_number4);
}

/******************************** Print Char **********************************/

int PrintChar2time(void *ch)
{
	static int repeat_number2 = 2;
	printf(" %c \n", *(char *)ch);

	fflush(stdout);

	--repeat_number2;

	return (!!repeat_number2);
}

/******************************** User Func Stop ******************************/
/*
int UserFuncStop(void *data)
{
	scheduler_ty *scheduler  = (scheduler_ty *)data;

	printf("func_stop\n");

	SchedulerStop(scheduler);

	return 0;
}
*/
/******************************** User Func Error *****************************/
/*
int UserFuncError(void *data)
{
	int num_to_print = *((int *)data);

	printf("%d\n", num_to_print);

	++user_func_error;

	if (2 == user_func_error)
	{
		return ERROR;
	}
	else
	{
		return NOT_COMPLETED;
	}
*/

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Scheduler Tests";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestInitializeScheduler();
	TestAddToScheduler(); 
	TestRemoveScheduler();
	TestRunScheduler();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
************************** Test Initialize Scheduler **************************/

/*	test case: checking all the list parameter after initializing
*/
void TestInitializeScheduler()
{ 
	const char *test_name = "INITIALIZE";

	scheduler_ty *scheduler = SchedulerCreate();
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.remove_status = SUCCESS;
	expected_results.status_of_execution = SUCCESS;

	results.is_empty = SchedulerIsEmpty(scheduler);
	results.size = SchedulerSize(scheduler);
	results.remove_status = SUCCESS;
	results.status_of_execution = SUCCESS;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	SchedulerDestroy(scheduler);
}

/*******************************************************************************
***************************** Test Add To Scheduler ***************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestAddToScheduler(void)
{ 
	const char *test_name = "ADD to";

	scheduler_ty *scheduler = SchedulerCreate();
	
	results_ty expected_results;
	results_ty results;

	char arry_of_char[NUM_OF_ELEMENTS] = {'A','B','C'};

	SchedulerAdd(scheduler, 1, PrintChar4time, (void *)arry_of_char);
	SchedulerAdd(scheduler, 5, PrintChar8time, (void *)(arry_of_char + 1));
	SchedulerAdd(scheduler, 8, PrintChar2time, (void *)(arry_of_char + 2));

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.remove_status = SUCCESS;
	expected_results.status_of_execution = SUCCESS;

	results.is_empty = SchedulerIsEmpty(scheduler);
	results.size = SchedulerSize(scheduler);
	results.remove_status = SUCCESS;
	results.status_of_execution = SUCCESS;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	SchedulerDestroy(scheduler);
}

/*******************************************************************************
***************************** Test Remove Scheduler ***************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestRemoveScheduler(void)
{ 

	const char *test_name = "REMOVE from";
	uid_ty uid_to_remove = {0};

	scheduler_ty *scheduler = SchedulerCreate();
	
	results_ty expected_results;
	results_ty results;

	char arry_of_char[NUM_OF_ELEMENTS] = {'A','B','C'};

	SchedulerAdd(scheduler, 1, PrintChar4time, (void *)arry_of_char);
	SchedulerAdd(scheduler, 5, PrintChar8time, (void *)(arry_of_char + 1));
	uid_to_remove = SchedulerAdd(scheduler, 8, PrintChar2time, (void *)(arry_of_char + 2));

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS -1;
	expected_results.remove_status = SUCCESS;
	expected_results.status_of_execution = SUCCESS;

	results.remove_status = SchedulerRemove(scheduler, uid_to_remove);
	results.is_empty = SchedulerIsEmpty(scheduler);
	results.size = SchedulerSize(scheduler);
	results.status_of_execution = SUCCESS;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	SchedulerDestroy(scheduler);
}

/*******************************************************************************
***************************** Test Run Scheduler ***************************/

/*	test case: checking all the list parameter after inserting n elements and 
               PQDequeue all of them.
*/

void TestRunScheduler(void)
{ 

	const char *test_name = "RUN";
	uid_ty uid_to_remove = {0};

	scheduler_ty *scheduler = SchedulerCreate();
	
	results_ty expected_results;
	results_ty results;

	char arry_of_char[NUM_OF_ELEMENTS] = {'A','B','C'};

	SchedulerAdd(scheduler, 1, PrintChar4time, (void *)arry_of_char);
	SchedulerAdd(scheduler, 5, PrintChar8time, (void *)(arry_of_char + 1));
	SchedulerAdd(scheduler, 8, PrintChar2time, (void *)(arry_of_char + 2));

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.remove_status = SUCCESS;
	expected_results.status_of_execution = SUCCESS;

	results.status_of_execution = SchedulerRun(scheduler);
	results.is_empty = SchedulerIsEmpty(scheduler);
	results.size = SchedulerSize(scheduler);
	results.remove_status = SUCCESS;

	TestResultComper(results, expected_results, test_name, __LINE__);

	SchedulerDestroy(scheduler);
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "Scheduler test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.is_empty != expected_results.is_empty)
	{
		printf(RED);
		printf("\nThe is_empty test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.size != expected_results.size)
	{
		printf(RED);
		printf("\nThe init counter test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.remove_status != expected_results.remove_status)
	{
		printf(RED);
		printf("\nThe Remove Status test fail for %s %s at line %lu\n", 
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.status_of_execution != expected_results.status_of_execution)
	{
		printf(RED);
		printf("\nThe Status of Execution test fail for %s %s at line %lu\n",
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








