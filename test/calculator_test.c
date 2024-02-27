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
#include "calculator.h"

/* colors for debugging */
#define RED			                     "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                            "\033[0;37m"

typedef enum status
{
	SUCCESS = 0,
	SYNTAX_ERROR, 
	MATH_ERROR, 
	SYS_ERROR
} status_ty; 

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

typedef struct results
{  
	int status;
	double result;
	
} results_ty;

/* test functions */
void TestCalculatorSimple(void);
void TestCalculatorSymbolError(void);
void TestCalculatorOnaryError(void);
void TestCalculatorWhiteSpaces(void);
void TestCalculatorParenthesis(void);
void TestCalculatorParenthesisError(void);
void TestCalculatorPower(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Calculator Tests";

	BeginingOfTests(tests_name);

	/* The main test functions*/ 
	TestCalculatorSimple();
	TestCalculatorSymbolError();
	TestCalculatorOnaryError();
	TestCalculatorWhiteSpaces();
	TestCalculatorParenthesis();
	TestCalculatorParenthesisError();
	TestCalculatorPower();

	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
*************************** Test Calculator Simple ****************************/

void TestCalculatorSimple(void)
{ 
	const char *test_name = "SIMPLE";
	const char str[100] = "2^2^3"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SUCCESS;
	expected_results.result = 256;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
************************ Test Calculator Symbol Error *************************/

void TestCalculatorSymbolError(void)
{ 
	const char *test_name = "SYMBOL ERROR";
	const char str[100] = "-6++2*2++B+-1/1*1"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SYNTAX_ERROR;
	expected_results.result = 0;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
*************************** Test Calculator Onary Error ****************************/

void TestCalculatorOnaryError(void)
{ 
	const char *test_name = "ONARY ERROR";
	const char str[100] = "-6++2*2++5+- 1/1*1"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SYNTAX_ERROR;
	expected_results.result = 0;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
************************* Test Calculator White Spaces ************************/

void TestCalculatorWhiteSpaces(void)
{ 
	const char *test_name = "WHITE SPACES";
	const char str[100] = "   6    +    2 * 2  "; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;


	expected_results.status = SUCCESS;
	expected_results.result = 10;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
************************* Test Calculator Parenthesis *************************/

void TestCalculatorParenthesis(void)
{ 
	const char *test_name = "PARENTHESIS";
	const char str[100] = "(4+(6+2))*(2+5)"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SUCCESS;
	expected_results.result = 84;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
************************* Test Calculator Parenthesis *************************/

void TestCalculatorParenthesisError(void)
{ 
	const char *test_name = "PARENTHESIS ERROR";
	const char str[100] = "(4+(6+2)())*(2+5)"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SYNTAX_ERROR;
	expected_results.result = 0;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
*************************** Test Calculator Power *****************************/

void TestCalculatorPower(void)
{ 
	const char *test_name = "Power";
	const char str[100] = "(4+(3*2))^(2^2*2)"; 

	double result = 0; 				

	results_ty expected_results;
	results_ty results;

	expected_results.status = SUCCESS;
	expected_results.result = 100000000;

	results.status = Calculate(str, &result);
	results.result = result;

	TestResultComper(results, expected_results, test_name, __LINE__);
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "Calculator test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.status != expected_results.status)
	{
		printf(RED);
		printf("\nThe status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
		printf(WHITE);
	}
	if(results.result != expected_results.result)
	{
		printf(RED);
		printf("\nThe result test fail for %s %s at line %lu\n",
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





