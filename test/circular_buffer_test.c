/******************************************************************************
* Project: Circular Buffer                                                    *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Doron                                                               *
*                                                                             *
******************************************************************************/ 
#include <unistd.h> /* ssize_t */
#include <stdio.h>  /* size_t */
#include <assert.h> /* assert */


#include "circular_buffer.h" 

#define ILRD_TRUE                        1
#define ILRD_FALSE                       0
#define ILRD_EMPTY                       0  

#define MIN2(a,b) (a < b) ? (a) : (b)             

typedef struct results
{  
	ssize_t read; 
    ssize_t write;
	size_t free_space;
	size_t size;	
    int is_empty;
	
}results_ty;

void CircBuffCreateTests(void);
void CircBuffFullTests(void);
void CircBuffWriteAndReadTests(void);
void CircBuffIsEmptyTests(void);
void EndOfTests(void);

/* helper */
static void TestResultCompare(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{ 	
	CircBuffCreateTests();
	CircBuffFullTests();
	CircBuffWriteAndReadTests();
	CircBuffIsEmptyTests();
	EndOfTests();

	return 0; 
}

/*******************************************************************************
************************ Circular Buffer Create Tests *************************/

/*	test case: checking all the Queue Buffer parameter after initializing
*/
void CircBuffCreateTests(void)
{ 
	const char *test_name = "Initialize Buffer Test";

	size_t capacity = 10; 
	cbuf_ty *cbuf = CircBuffCreate(capacity);
	results_ty expected_results;
	results_ty results;

	results.read = ILRD_EMPTY;
	results.write = ILRD_EMPTY;
	results.free_space = CircBuffFreeSpace(cbuf);
	results.size = CircBuffBufsiz(cbuf);
	results.is_empty = CircBuffIsEmpty(cbuf);

	expected_results.read = ILRD_EMPTY;
	expected_results.write = ILRD_EMPTY;
	expected_results.free_space = capacity;
	expected_results.size = ILRD_EMPTY;
	expected_results.is_empty = ILRD_TRUE;

	CircBuffDestroy(cbuf);

	TestResultCompare(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
************************ Circular Buffer Full Tests ***************************/

/*	test case:  checking all the Queue Buffer parameter after initializing and 
				fill the buffer capacity bytes 
*/
void CircBuffFullTests(void)
{ 
	const char *test_name = "Full Buffer Test";

	size_t capacity = 10; 
	size_t num_to_fill = 10;
	cbuf_ty *cbuf = CircBuffCreate(capacity);

	char *src = "hello world, this is a test!!"; 
	
	results_ty expected_results;
	results_ty results;

	results.read = ILRD_EMPTY;
	results.write = CircBuffWrite(cbuf, src, num_to_fill);
	results.free_space = CircBuffFreeSpace(cbuf);
	results.size = CircBuffBufsiz(cbuf);
	results.is_empty = CircBuffIsEmpty(cbuf);

	expected_results.read = ILRD_EMPTY;	
	expected_results.write = num_to_fill;
	expected_results.free_space = ILRD_EMPTY;
	expected_results.size =  capacity;
	expected_results.is_empty = ILRD_FALSE;
		
	CircBuffDestroy(cbuf);

	TestResultCompare(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************* Circular Buffer Write And Read Tests **********************/

/*	test case: checking all the Queue parameter after initializing
*/
void CircBuffWriteAndReadTests(void)
{ 
	const char *test_name = "Write And Read Test";

	size_t capacity = 10; 
	size_t num_to_fill = 11;
	cbuf_ty *cbuf = CircBuffCreate(capacity);
	char *src = "123456789101112"; 
	char dst[10] = {""};
	results_ty expected_results;
	results_ty results;


	printf("\n%ld",CircBuffFreeSpace(cbuf));
	printf("\n%ld",CircBuffBufsiz(cbuf));

	CircBuffWrite(cbuf, src, 5);


	printf("\n%ld",CircBuffFreeSpace(cbuf));

	
	results.read = CircBuffRead(cbuf, dst, 5);
	results.write = CircBuffWrite(cbuf, src, num_to_fill);
	results.free_space = CircBuffFreeSpace(cbuf);
	results.size = CircBuffBufsiz(cbuf);
	results.is_empty = CircBuffIsEmpty(cbuf);

	expected_results.read = 5;	
	expected_results.write = num_to_fill -1;
	expected_results.free_space = capacity -num_to_fill +1;
	expected_results.size =  capacity;
	expected_results.is_empty = ILRD_FALSE;

	CircBuffDestroy(cbuf);

	TestResultCompare(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************* Circular Buffer Write And Read Tests **********************/

/*	test case: checking all the Queue parameter after initializing
*/
void CircBuffIsEmptyTests(void)
{ 
	const char *test_name = "Is Empty Test";

	size_t capacity = 10; 
	size_t num_to_fill = 10;
	cbuf_ty *cbuf = CircBuffCreate(capacity);
	char *src = "123456789101112"; 
	char dst[10] = {""};
	results_ty expected_results;
	results_ty results;

	results.write = CircBuffWrite(cbuf, src, num_to_fill);
	results.read = CircBuffRead(cbuf, dst, num_to_fill);
	results.free_space = CircBuffFreeSpace(cbuf);
	results.size = CircBuffBufsiz(cbuf);
	results.is_empty = CircBuffIsEmpty(cbuf);

	expected_results.read = num_to_fill;	
	expected_results.write = num_to_fill;
	expected_results.free_space = capacity;
	expected_results.size =  ILRD_EMPTY;
	expected_results.is_empty = ILRD_TRUE;

	CircBuffDestroy(cbuf);

	TestResultCompare(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
****************************** Test Result Compare ****************************/

static void TestResultCompare(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line)
{ 
	int test_is_successful_flag = ILRD_TRUE;

	if(results.read != expected_results.read)
	{
		printf("\nThe read test fail for %s at line %lu\n",
		                                                       test_name, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.write != expected_results.write)
	{
		printf("\nThe write fail for %s at line %lu\n",
		                                                       test_name, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.free_space != expected_results.free_space)
	{
		printf("\nThe free_space test fail for %s at line %lu\n",
		                                                       test_name, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.size != expected_results.size)
	{
		printf("\nThe size test fail for %s at line %lu\n",
		                                                       test_name, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.is_empty != expected_results.is_empty)
	{
		printf("\nThe is_empty test fail for %sat line %lu\n",
		                                                       test_name, line);
		test_is_successful_flag = ILRD_FALSE;
	} 

	if(test_is_successful_flag == ILRD_TRUE)
	{
		printf("\n %s passed !\n", test_name);
	}
	
}

/*******************************************************************************
********************************** End Of Tests *******************************/

void EndOfTests(void)
{
	printf("\n_________________________End Of Tests________________________\n");
} 











