/******************************************************************************
* RBST_test.c                                                                *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                                 *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
#include "rbst.h"

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10
#define NUM_OF_ELEMENTS_3     3
#define LARGE_NUMBER       1000
#define NO_HEIGHT             0
#define NO_ROOT               0

/* colors for debugging */
#define RED			                     "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                            "\033[0;37m"

enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 


typedef struct results
{  
	int is_empty;
	size_t size;
	size_t height;
	void * data;
	
} results_ty;

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum is_empty 
{
	NOT_EMPTY = 0, 
	EMPTY  = 1 
};
              
enum position
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};

struct node
{
    void *data;
    size_t height;
    node_ty *children[NUM_OF_CHILDREN];
};
 
struct rbst
{
    node_ty *root;
    cmp_func_ty cmp_func;
};

enum compare
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

/* test functions */
void TestInitializeRBST(void);
void TestFullRBST(void);
void TestInsertSimpleBalance(void);
void TestInsertLargeBalance(void);
void TestRBSTRemove(void); 


/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/* helper */
static void RBSTMultiInsert(rbst_ty *rbst, size_t num_of_insert, int *array);
static void RBSTMultiRemove(rbst_ty *rbst, size_t num_of_remove, int *array);

/* action & match functions */

int int_comp(void* data, void *param);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Recursive Binary Search Tree Tests";

	BeginingOfTests(tests_name);

	/* The main test functions*/ 
	TestInitializeRBST();
	TestFullRBST();
	TestInsertSimpleBalance();
	TestInsertLargeBalance();
	TestRBSTRemove();
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
************************* Test Initialize Double List *************************/

/*	test case: checking all the rbst parameter after initializing
*/
void TestInitializeRBST(void)
{ 
	const char *test_name = "INITIALIZE";

	rbst_ty *rbst = RBSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.height = NO_HEIGHT;
	expected_results.data = &test_name;

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = NO_HEIGHT;
	results.data = &test_name;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	RBSTDestroy(rbst);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the rbst parameter after inserting n elements
*/

void TestFullRBST(void)
{ 
	const char *test_name = "FULL";

	rbst_ty *rbst = RBSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {1,2,3,4,5,6,7,8,9,10};

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.height = NO_HEIGHT;
	expected_results.data = &test_name;

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = NO_HEIGHT;
	results.data = &test_name;

	RBSTDestroy(rbst);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the rbst parameter after inserting n elements
*/

void TestInsertSimpleBalance(void)
{ 
	const char *test_name_rl = "Insert Right-Left balance simple case";
	const char *test_name_rr = "Insert Right-Right balance simple case";
	const char *test_name_lr = "Insert Left-Right balance simple case";
	const char *test_name_ll = "Insert Left-Left balance simple case";

	rbst_ty *rbst = RBSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	/*________________________________ Right-Left ___________________________ */

	int array_of_data[NUM_OF_ELEMENTS_3] = {3,5,4};

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS_3;
	expected_results.height = 1;
	expected_results.data = (void *)&array_of_data[2];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;

	TestResultComper(results, expected_results, test_name_rl, __LINE__);

	RBSTMultiRemove(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	/*________________________________ Right-Right __________________________ */


	array_of_data[0] = 3;
	array_of_data[1] = 4;
	array_of_data[2] = 5;

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS_3;
	expected_results.height = 1;
	expected_results.data = (void *)&array_of_data[1];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;
		
	TestResultComper(results, expected_results, test_name_rr, __LINE__);

	RBSTMultiRemove(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	/*________________________________ Left-Right ___________________________ */


	array_of_data[0] = 5;
	array_of_data[1] = 3;
	array_of_data[2] = 4;

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS_3;
	expected_results.height = 1;
	expected_results.data = (void *)&array_of_data[2];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;
		
	TestResultComper(results, expected_results, test_name_lr, __LINE__);

	RBSTMultiRemove(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	/*________________________________ Left-Left ____________________________ */

	array_of_data[0] = 5;
	array_of_data[1] = 4;
	array_of_data[2] = 3;

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS_3;
	expected_results.height = 1;
	expected_results.data = (void *)&array_of_data[1];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;
		
	TestResultComper(results, expected_results, test_name_ll, __LINE__);

	RBSTMultiRemove(rbst, NUM_OF_ELEMENTS_3, array_of_data);

	RBSTDestroy(rbst);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the rbst parameter after inserting n elements
*/

void TestInsertLargeBalance(void)
{ 
	const char *test_name = "Large scale balance";


	rbst_ty *rbst = RBSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	int i = 0;

	int array_of_data[LARGE_NUMBER];

	for(i = 0; i < LARGE_NUMBER; i++)
	{
		array_of_data[i] = i+1;
	}

	RBSTMultiInsert(rbst, LARGE_NUMBER, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = LARGE_NUMBER;
	expected_results.height = 12;
	expected_results.data = (void *)&array_of_data[485];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;

	TestResultComper(results, expected_results, test_name, __LINE__);

	RBSTDestroy(rbst);
}

/*******************************************************************************
********************************* Test rbst Remove *****************************/

/*	test case: checking all the parameters after inserting 
    n elements to the rbst, and removing m element. 
*/

void TestRBSTRemove(void)
{ 
	const char *test_name_remove_1 = "REMOVE LEAF";

	rbst_ty *rbst = RBSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {8,6,14,5,7,16,1,10,17,12};
	int array_to_remove[3] = {7, 6, 5};

	RBSTMultiInsert(rbst, NUM_OF_ELEMENTS, array_of_data);

	/*________________________________ remove leaf __________________________ */


	RBSTRemove(rbst, &array_to_remove[0]);
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS -1;
	expected_results.height = 3;
	expected_results.data = (void *)&array_of_data[0];

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);
	results.height = RBSTHeight(rbst);
	results.data = rbst->root->data;

	TestResultComper(results, expected_results, test_name_remove_1, __LINE__);

	/*___________________________ remove node with child ____________________ */

/*
	RBSTRemove(RBSTFind(rbst, (void *)(&array_to_remove[1])));
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS -2;

	results.is_empty = RBSTIsEmpty(rbst);
	results.size = RBSTSize(rbst);

	TestResultComper(results, expected_results, test_name_remove_2, __LINE__);*/

	RBSTDestroy(rbst);
}

/*******************************************************************************
****************************** rbst Multi Insert *****************************/

static void RBSTMultiInsert(rbst_ty *rbst, size_t num_of_insert, int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		RBSTInsert(rbst, (void *)(array + i));
	}
}

/*******************************************************************************
****************************** rbst Multi Remove *****************************/

static void RBSTMultiRemove(rbst_ty *rbst, size_t num_of_remove, int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_remove; i++)
	{
		RBSTRemove(rbst, (void *)(array + i));
	}
}

/*******************************************************************************
********************************** Int Compare ********************************/

int int_comp(void* data, void *param)
{
	if(*(int*)data > *(int*)param)
	{
		return BIGGER;
	}
	else if(*(int*)data < *(int*)param)
	{
		return SMALLER;
	}

	return EQUAL;
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "recursive binary search tree test";
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
		printf("\nThe size test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.height != expected_results.height)
	{
		printf(RED);
		printf("\nThe height test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(*(int *)results.data != *(int *)expected_results.data)
	{
		printf(RED);
		printf("\nThe data test fail for %s %s at line %lu\n",
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








