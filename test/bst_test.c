/******************************************************************************
* BST_test.c                                                                *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Eli                                                                 *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
#include "bst.h"

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10
#define SRC_SIZE              7 
#define DEST_SIZE             3

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
              
typedef struct results
{  
	int is_empty;
	size_t size;
	bst_iter_ty begin;
    bst_iter_ty end;		
    int next;
    int prev;
	
} results_ty;


typedef struct fun_results
{  
	int status;
	bst_iter_ty return_ptr;

} fun_results_ty;

enum position
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};

struct node
{
    void *data;
    node_ty *children[NUM_OF_CHILDREN];
    node_ty *parent; 
};
 
struct bst
{
    node_ty pre_root;
    cmp_func_ty cmp_func;
};

enum compare
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
};

/* test functions */
void TestInitializeBST(void);
void TestFullBST(void);
void TestBSTRemove(void); 
void TestMultiFindFunctionBST(void);
void TestForEachFunction(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static int CheckBSTSequenceOfNext(bst_ty *bst, int *array, size_t size);
static int CheckBSTSequenceOfPrev(bst_ty *bst, int *array, size_t size);
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void FunResultComper(fun_results_ty fun_results, 
	   fun_results_ty expected_fun_results, const char *test_name, size_t line);

/* helper */
static bst_iter_ty BSTMultiNext(bst_iter_ty iter, size_t num_of_next);
static bst_iter_ty BSTMultiPrev(bst_iter_ty iter, size_t num_of_next);
static void BSTMultiInsert(bst_ty *bst, size_t num_of_insert, int *array);

/* action & match functions */

int int_comp(void* data, void *param);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Binary Search Tree Tests";

	BeginingOfTests(tests_name);

	/* The main test functions*/ 
	TestInitializeBST();
	TestFullBST();
	TestBSTRemove();
	/*TestSpliceBST();
	TestMultiFindFunctionBST();
	TestForEachFunction(); */
	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
************************* Test Initialize Double List *************************/

/*	test case: checking all the BST parameter after initializing
*/
void TestInitializeBST(void)
{ 
	const char *test_name = "INITIALIZE";

	bst_ty *bst = BSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	expected_results.is_empty = EMPTY;
	expected_results.size = COUNTER_EMPTY;
	expected_results.begin = &(bst->pre_root);
	expected_results.end = &(bst->pre_root);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = BSTIsEmpty(bst);
	results.size = BSTSize(bst);
	results.begin = BSTBegin(bst);
	results.end = BSTEnd(bst);
	results.next = SUCSSES;
	results.prev = SUCSSES;
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	BSTDestroy(bst);
}

/*******************************************************************************
**************************** Test Full Double List ****************************/

/*	test case: checking all the BST parameter after inserting n elements
*/

void TestFullBST(void)
{ 
	const char *test_name = "FULL";

	bst_ty *bst = BSTCreate(int_comp);
	
	results_ty expected_results;
	results_ty results;

	int array_of_data[10] = {1,2,3,4,5,6,7,8,9,10};

	BSTMultiInsert(bst, NUM_OF_ELEMENTS, array_of_data);

	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS;
	expected_results.begin = BSTBegin(bst);
	expected_results.end = BSTMultiNext(BSTBegin(bst),NUM_OF_ELEMENTS);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = BSTIsEmpty(bst);
	results.size = BSTSize(bst);
	results.begin = BSTBegin(bst);
	results.end = BSTEnd(bst);
	results.next = CheckBSTSequenceOfNext(bst, array_of_data, NUM_OF_ELEMENTS);
	results.prev = CheckBSTSequenceOfPrev(bst, array_of_data, NUM_OF_ELEMENTS -1);

	BSTDestroy(bst);
		
	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
********************************* Test BST Remove *****************************/

/*	test case: checking all the parameters after inserting 
    n elements to the bst, and removing m element. 
*/

void TestBSTRemove(void)
{ 
	const char *test_name_remove_1 = "REMOVE LEAF";
	const char *test_name_remove_2 = "REMOVE NODE WITH ONE CHILD";
	const char *test_name_remove_3 = "REMOVE NODE WITH TWO CHILD"; 
	const char *test_name_empty  = "EMPTY";

	results_ty expected_results;
	results_ty results;

	int array_of_data[NUM_OF_ELEMENTS] = {5,3,4,2,6,10,11,8,9,7};
	int exp_remove_leaf[NUM_OF_ELEMENTS - 1] = {2,3,4,5,6,7,8,9,10};
	int exp_node_one_child[NUM_OF_ELEMENTS - 2] = {2,3,4,5,7,8,9,10};
	int exp_node_two_child[NUM_OF_ELEMENTS - 3] = {2,3,4,7,8,9,10};
	int array_to_remove[3] = {11, 6, 5};
	int array_remove[1] = {7};
	int i = 0; 

	bst_ty *bst = BSTCreate(int_comp);

	BSTMultiInsert(bst, NUM_OF_ELEMENTS, array_of_data);

	/*________________________________ remove leaf __________________________ */


	BSTRemove(BSTFind(bst, (void *)(&array_to_remove[0])));
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS -1;
	expected_results.begin = BSTBegin(bst);
	expected_results.end = BSTMultiNext(BSTBegin(bst),NUM_OF_ELEMENTS -1);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = BSTIsEmpty(bst);
	results.size = BSTSize(bst);
	results.begin = BSTBegin(bst);
	results.end = BSTEnd(bst);
	results.next = CheckBSTSequenceOfNext(bst, exp_remove_leaf, NUM_OF_ELEMENTS -1);
	results.prev = CheckBSTSequenceOfPrev(bst, exp_remove_leaf, NUM_OF_ELEMENTS -1);

	TestResultComper(results, expected_results, test_name_remove_1, __LINE__);

	/*___________________________ remove node with child ____________________ */


	BSTRemove(BSTFind(bst, (void *)(&array_to_remove[1])));
	
	expected_results.is_empty = NOT_EMPTY;
	expected_results.size = NUM_OF_ELEMENTS -2;
	expected_results.begin = BSTBegin(bst);
	expected_results.end = BSTMultiNext(BSTBegin(bst),NUM_OF_ELEMENTS -2);
	expected_results.next = SUCSSES;
	expected_results.prev = SUCSSES;

	results.is_empty = BSTIsEmpty(bst);
	results.size = BSTSize(bst);
	results.begin = BSTBegin(bst);
	results.end = BSTEnd(bst);
	results.next = CheckBSTSequenceOfNext(bst, exp_remove_leaf, NUM_OF_ELEMENTS -2);
	results.prev = CheckBSTSequenceOfPrev(bst, exp_remove_leaf, NUM_OF_ELEMENTS -2);

	TestResultComper(results, expected_results, test_name_remove_2, __LINE__);

}





/*******************************************************************************
****************************** BST Multi Insert *****************************/

static void BSTMultiInsert(bst_ty *bst, size_t num_of_insert, int *array)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_insert; i++)
	{
		BSTInsert(bst, (void *)(array + i));
	}
}

/*******************************************************************************
****************************** BST Multi next *******************************/

static bst_iter_ty BSTMultiNext(bst_iter_ty iter, size_t num_of_next)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_next; i++)
	{
		iter = BSTNext(iter);
	}

	return iter;
}

/*******************************************************************************
****************************** BST Multi prev *******************************/

static bst_iter_ty BSTMultiPrev(bst_iter_ty iter, size_t num_of_next)
{
	size_t i = 0;
	
	for(i = 0; i < num_of_next; i++)
	{
		iter = BSTPrev(iter);
	}

	return iter;
} 

/*******************************************************************************
************************** Check BST Sequence Of Next ***********************/

static int CheckBSTSequenceOfNext(bst_ty *bst, int *array_of_data, size_t size)
{
	int status = SUCSSES;
	size_t i = 0;  

	bst_iter_ty iter = BSTBegin(bst);

	/* testing the next type of the nodes */
	for(i = 0 ; i < size ; i++)
	{
		if(*(int *)(iter->data) != *(array_of_data + i))
		{
			status = FAILURE; 
			break; 
		}

		iter = BSTNext(iter); 
	}
	return status;
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
************************** Check BST Sequence Of Prev ***********************/

static int CheckBSTSequenceOfPrev(bst_ty *bst, int *array_of_data, size_t size)
{
	int status = SUCSSES;
	size_t i = 0;
	bst_iter_ty iter = BSTMultiNext(BSTBegin(bst),NUM_OF_ELEMENTS - 2);  

	array_of_data += size - 1; 

	/* testing the prev type of the nodes */
	for(i = 0 ; i < size - 1 ; i++)
	{
		if(*(int *)(iter->data) != *(array_of_data - i))
		{
			status = FAILURE; 
			break; 
		}

		iter = BSTPrev(iter); 
	} 

	return status; 
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "binary search tree test";
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
	if(results.begin != expected_results.begin)
	{
		printf(RED);
		printf("\nThe results.begin test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.end != expected_results.end)
	{
		printf(RED);
		printf("\nThe results.end test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	} 
	if(results.next != expected_results.next)
	{
		printf(RED);
		printf("\nThe Sequence of Next's test fail for %s %s at line %lu\n", 
			                                      test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.prev != expected_results.prev)
	{
		printf(RED);
		printf("\nThe Sequence of Prev's test fail for %s %s at line %lu\n",
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
******************************* Test Result Comper ****************************/

static void FunResultComper(fun_results_ty fun_results, 
	    fun_results_ty expected_fun_results, const char *test_name, size_t line)
{ 
	const char *test_object= "function results";
	int test_is_successful_flag = ILRD_TRUE;

	if(expected_fun_results.status != fun_results.status)
	{
		printf(RED);
		printf("\nThe status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(expected_fun_results.return_ptr != fun_results.return_ptr)
	{
		printf(RED);
		printf("\nThe init counter test fail for %s %s at line %lu\n",
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








