/******************************************************************************
* Project : DHCP                                                              *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Liel                                                                *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */
#include <string.h> /* memcpy, strcmp */
#include <math.h>  /* pow */


#include "dhcp.h"
#include "utils.h"

#define ASCII_TABLE_SIZE    256 

#define COUNTER_EMPTY         0 
#define NUM_OF_ELEMENTS      10
#define IRRELEVANT           77

#define BYTES_IN_IPV          4

typedef struct results
{  
	size_t count_free;
	DHCP_status_ty alloc_status;
	DHCP_status_ty free_status;		
   unsigned char returned_address[BYTES_IN_IPV];
	
} results_ty;

/* test functions */
void TestDHCPCreate(void);
void TestDHCPAllocateSuccessIp(void);
void TestDHCPAllocateDiffIp(void);
void TestDHCPFreeIp(void);
void TestDHCPFreeFailureIp(void);
void TestDHCPAllocateAndFreeIp(void);
void TestDHCPFULL(void);

/* print start & end of tests functions */
static void BeginingOfTests(const char *test_name);
static void EndOfTests(void);
static void Remainder(void);

/* copmpare functions */
static void TestResultComper(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static int TestResultComperMulti(results_ty results, results_ty expected_results,
                                            const char *test_name, size_t line);
static void TestPrintResultComperMulti(int test_is_successful_flag, 
	                                                     const char *test_name);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	const char *tests_name = "Heap Data Structre Tests";

	BeginingOfTests(tests_name);

	/* The main test functions */ 
	TestDHCPCreate();
	TestDHCPAllocateSuccessIp();
	TestDHCPAllocateDiffIp();
	TestDHCPFreeIp();
	TestDHCPFreeFailureIp();
	/*TestDHCPAllocateAndFreeIp();
	TestDHCPFULL();*/

	Remainder();

	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
******************************* Test DHCP Create ******************************/

void TestDHCPCreate(void)
{ 
	const char *test_name = "CREATE";
	unsigned char network_address[BYTES_IN_IPV] = {165,10,10,10};
	unsigned char irrelevant[BYTES_IN_IPV] = {0,7,7,7};
	size_t subnet_mask = 20; 

	dhcp_ty *dhcp = DHCPCreate(network_address, subnet_mask);
	
	results_ty expected_results;
	results_ty results;

	expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - 3;

	expected_results.alloc_status = IRRELEVANT;
	expected_results.free_status  = IRRELEVANT;
	memcpy(expected_results.returned_address, irrelevant, BYTES_IN_IPV);

	results.count_free = DHCPCountFree(dhcp);

	results.alloc_status = IRRELEVANT;
	results.free_status  = IRRELEVANT;
	memcpy(results.returned_address, irrelevant, BYTES_IN_IPV);
		
	TestResultComper(results, expected_results, test_name, __LINE__);

	DHCPDestroy(dhcp);
}

/*******************************************************************************
******************************* Test DHCP Allocate ****************************/

void TestDHCPAllocateSuccessIp()
{ 
	const char *test_name = "ALLOCATE SUCCES";
	unsigned char network_address[BYTES_IN_IPV] = {165,10,10,10};
	size_t subnet_mask = 24;
	int test_flag = ILRD_TRUE; 

	dhcp_ty *dhcp = DHCPCreate(network_address, subnet_mask);
	
	results_ty expected_results;
	results_ty results;

	unsigned char requested_ip[NUM_OF_ELEMENTS][BYTES_IN_IPV] =  {{165,10,10,6},{165,10,10,20},
	                                                     {165,10,10,10}, {165,10,10,30},
	                                                     {165,10,10,40}, {165,10,10,100},
	                                                     {165,10,10,200},{165,10,10,80},
	                                                     {165,10,10,70},{165,10,10,240}};

	unsigned char returned_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = {{0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0}};

	unsigned char expected_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = {{165,10,10,6},{165,10,10,20},
	                                                    	 {165,10,10,10}, {165,10,10,30},
	                                                     	{165,10,10,40}, {165,10,10,100},
	                                                     	{165,10,10,200},{165,10,10,80},
	                                                    	 {165,10,10,70},{165,10,10,240}};
	int i = 0; 	
	
	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		results.alloc_status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - 4 - i;
		expected_results.alloc_status = SUCCESS;
		expected_results.free_status  = IRRELEVANT;
		memcpy(expected_results.returned_address, expected_address[i], BYTES_IN_IPV);


		results.count_free = DHCPCountFree(dhcp);
		results.free_status  = IRRELEVANT;
		memcpy(results.returned_address, returned_address[i], BYTES_IN_IPV);

		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	DHCPDestroy(dhcp);
}

/*******************************************************************************
******************************* Test DHCP Allocate ****************************/

void TestDHCPAllocateDiffIp()
{ 
	const char *test_name = "ALLOCATE DIFRRENT";
	unsigned char network_address[BYTES_IN_IPV] = {165,10,10,10};
	size_t subnet_mask = 24;
	int test_flag = ILRD_TRUE; 

	dhcp_ty *dhcp = DHCPCreate(network_address, subnet_mask);
	
	results_ty expected_results;
	results_ty results;

	unsigned char requested_ip[NUM_OF_ELEMENTS][BYTES_IN_IPV] =  
											   {{165,10,10,1},	{165,10,10,0},
	                                            {165,10,10,1}, 	{165,10,10,255},
	                                            {165,10,10,0}, 	{255,10,10,255},
	                                            {215,10,11,2}, 	{212,10,13,80},
	                                            {170,10,10,2}, 	{166,10,10,240}};

	unsigned char returned_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = 
											   {{0,0,0,0},	{0,0,0,0},
	                                           	{0,0,0,0},	{0,0,0,0},
	                                            {0,0,0,0},	{0,0,0,0},
	                                            {0,0,0,0},	{0,0,0,0},
	                                            {0,0,0,0},	{0,0,0,0}};

	unsigned char expected_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = 
											   {{165,10,10,3},	{165,10,10,2},
	                                            {165,10,10,5},	{165,10,10,254},
	                                            {165,10,10,4},	{165,10,10,253},
	                                            {165,10,10,6},	{165,10,10,80},
	                                            {165,10,10,7},	{165,10,10,240}};
	int i = 0; 


	for(i = 0; i < NUM_OF_ELEMENTS / 2; ++i)
	{
		results.alloc_status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - 4 - i;
		expected_results.alloc_status = DIFFERENT_ADDRESS;
		expected_results.free_status  = IRRELEVANT;
		memcpy(expected_results.returned_address, expected_address[i], BYTES_IN_IPV);

		results.count_free = DHCPCountFree(dhcp);
		results.free_status  = IRRELEVANT;
		memcpy(results.returned_address, returned_address[i], BYTES_IN_IPV);

		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}
	
	for(i = NUM_OF_ELEMENTS / 2; i < NUM_OF_ELEMENTS; ++i)
	{
		results.alloc_status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - 4 - i;
		expected_results.alloc_status = INVALID_ADDRESS;
		expected_results.free_status  = IRRELEVANT;
		memcpy(expected_results.returned_address, expected_address[i], BYTES_IN_IPV);

		results.count_free = DHCPCountFree(dhcp);
		results.free_status  = IRRELEVANT;
		memcpy(results.returned_address, returned_address[i], BYTES_IN_IPV);

		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}

	TestPrintResultComperMulti(test_flag/(NUM_OF_ELEMENTS), test_name);

	DHCPDestroy(dhcp);
}
/*******************************************************************************
********************************* Test DHCP Free ******************************/

void TestDHCPFreeIp()
{ 
	const char *test_name = "FREE";
	unsigned char network_address[BYTES_IN_IPV] = {165,10,10,10};
	unsigned char irrelevant[BYTES_IN_IPV] = {0,7,7,7};
	size_t subnet_mask = 24;
	int test_flag = ILRD_TRUE; 

	dhcp_ty *dhcp = DHCPCreate(network_address, subnet_mask);
	
	results_ty expected_results;
	results_ty results;

	unsigned char requested_ip[NUM_OF_ELEMENTS][BYTES_IN_IPV] = {{165,10,10,6},	{165,10,10,20},
	                                                    {165,10,10,10}, {165,10,10,30},
	                                                    {165,10,10,40}, {165,10,10,100},
	                                                    {165,10,10,200},{165,10,10,80},
	                                                    {165,10,10,70},	{165,10,10,240}};

	unsigned char returned_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = {{0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0},
	                                                        {0,0,0,0},{0,0,0,0}};
	
	int i = 0;

	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		results.alloc_status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);
	}

	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		results.free_status  = DHCPFreeIp(dhcp, requested_ip[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - NUM_OF_ELEMENTS + i - 2;
		expected_results.alloc_status = IRRELEVANT;
		expected_results.free_status  = ILRD_SUCSSES;
		memcpy(expected_results.returned_address, irrelevant, BYTES_IN_IPV);

		results.count_free = DHCPCountFree(dhcp);
		results.alloc_status = IRRELEVANT;
		memcpy(results.returned_address, irrelevant, BYTES_IN_IPV);


		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	DHCPDestroy(dhcp);
}


/*******************************************************************************
***************************** Test DHCP Free Failure **************************/

void TestDHCPFreeFailureIp()
{ 
	const char *test_name = "FREE FAILURE";
	unsigned char network_address[BYTES_IN_IPV] = {165,10,10,10};
	unsigned char irrelevant[BYTES_IN_IPV] = {0,7,7,7};
	size_t subnet_mask = 24;
	int test_flag = ILRD_TRUE; 

	dhcp_ty *dhcp = DHCPCreate(network_address, subnet_mask);
	
	results_ty expected_results;
	results_ty results;

	unsigned char requested_ip[NUM_OF_ELEMENTS][BYTES_IN_IPV] =  
										   {{165,10,10,6},  {165,10,10,20},
	                                        {165,10,10,10}, {165,10,10,30},
	                                        {165,10,10,40}, {165,10,10,100},
	                                        {165,10,10,200},{165,10,10,80},
	                                        {165,10,10,70}, {165,10,10,240}};

	unsigned char returned_address[NUM_OF_ELEMENTS][BYTES_IN_IPV] = 
										   {{0,0,0,0},	{0,0,0,0},
	                                        {0,0,0,0},	{0,0,0,0},
	                                        {0,0,0,0},	{0,0,0,0},
	                                        {0,0,0,0},	{0,0,0,0},
	                                        {0,0,0,0},	{0,0,0,0}};

	unsigned char address_to_free[NUM_OF_ELEMENTS][BYTES_IN_IPV] = 
										   {{166,10,10,6},	{205,10,10,20},
	                                        {167,12,10,10}, {165,13,13,30},
	                                        {140,65,10,40}, {165,10,10,4},
	                                        {165,10,10,250},{165,10,10,81},
	                                        {165,10,10,78},	{165,10,10,241}};
	
	int i = 0;

	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		results.alloc_status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);
	}

	for(i = 0; i < NUM_OF_ELEMENTS / 2 ; ++i)
	{
		results.free_status  = DHCPFreeIp(dhcp, address_to_free[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - NUM_OF_ELEMENTS - 3;
		expected_results.alloc_status = IRRELEVANT;
		expected_results.free_status  = INVALID_ADDRESS;
		memcpy(expected_results.returned_address, irrelevant, BYTES_IN_IPV);

		results.count_free = DHCPCountFree(dhcp);
		results.alloc_status = IRRELEVANT;
		memcpy(results.returned_address, irrelevant, BYTES_IN_IPV);


		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}

	for(i = NUM_OF_ELEMENTS / 2; i < NUM_OF_ELEMENTS; ++i)
	{
		results.free_status  = DHCPFreeIp(dhcp, address_to_free[i]);

		expected_results.count_free = (1 << (BYTES_IN_IPV*8 - subnet_mask)) - NUM_OF_ELEMENTS - 3;
		expected_results.alloc_status = IRRELEVANT;
		expected_results.free_status  = DOUBLE_FREE;
		memcpy(expected_results.returned_address, irrelevant, BYTES_IN_IPV);

		results.count_free = DHCPCountFree(dhcp);
		results.alloc_status = IRRELEVANT;
		memcpy(results.returned_address, irrelevant, BYTES_IN_IPV);


		test_flag += TestResultComperMulti(results, expected_results, 
			                               test_name, __LINE__);		
	}

	TestPrintResultComperMulti(test_flag/NUM_OF_ELEMENTS, test_name);

	DHCPDestroy(dhcp);
}

/*******************************************************************************
******************************* Test Result Comper ****************************/

static void TestResultComper(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "dhcp test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.count_free != expected_results.count_free)
	{
		printf(RED);
		printf("\nThe count_free test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.alloc_status != expected_results.alloc_status)
	{
		printf(RED);
		printf("\nThe alloc_status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}

	if(results.free_status != expected_results.free_status)
	{
		printf(RED);
		printf("\nThe free_status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(0 != strncmp((const char *)results.returned_address, 
		            (const char *)expected_results.returned_address, BYTES_IN_IPV))
	{
		printf(RED);
		printf("\nThe returned_address test fail for %s %s at line %lu\n",
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
**************************** Test Result Comper Multi *************************/

static int TestResultComperMulti(results_ty results, results_ty expected_results,
                                             const char *test_name, size_t line)
{ 
	const char *test_object= "dhcp test";
	int test_is_successful_flag = ILRD_TRUE;

	if(results.count_free != expected_results.count_free)
	{
		printf(RED);
		printf("\nThe count_free test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(results.alloc_status != expected_results.alloc_status )
	{
		printf(RED);
		printf("\nThe alloc_status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}

	if(results.free_status != expected_results.free_status)
	{
		printf(RED);
		printf("\nThe free_status test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}
	if(0 != strncmp((const char *)results.returned_address, 
		            (const char *)expected_results.returned_address, BYTES_IN_IPV))
	{
		printf(RED);
		printf("\nThe returned_address test fail for %s %s at line %lu\n",
		                                          test_name, test_object, line);
		test_is_successful_flag = ILRD_FALSE;
	}

	return test_is_successful_flag;
}

/*******************************************************************************
************************ Test Print Result Comper Multi ***********************/

static void TestPrintResultComperMulti(int test_is_successful_flag,
                                       const char *test_name)
{ 
	const char *test_object= "dhcp test";

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








