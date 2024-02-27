/******************************************************************************
*	vsa_test - Unit Test
*	authour : Nisan Ohana
*	reviewer : Nasrat
*
*******************************************************************************/
#include <stdio.h> /*printf, puts*/

#include "variable_size_allocator.h"
#include "utills.h"
/**********************************MACROS**************************************/
#define VSA_INSTANCES 4
#define NUM_OF_ALLOCS 5
/**************************HelperFucntionsDeclarations*************************/
static void AllocateSpace(void **pointer_arr, size_t size_of_arr, size_t *space);
static void ClearSpace(void **pointer_arr, size_t size_of_arr);
/**************************FucntionsDeclarations*******************************/
void TestVSAInit(char *test_name);
void TestVSAAlloc(char *test_name);
void TestVSAFree(char *test_name);
void TestVSAFree2(char *test_name);
/***********************************MAIN***************************************/
int main()
{
	TestVSAInit("VSAInit");
	TestVSAAlloc("VSAAlloc");
	TestVSAFree("VSAFree");
	TestVSAFree2("VSAFree2");
	return 0;
}
/************************TestVSAInit*******************************************/
void TestVSAInit(char *test_name)
{
	size_t i = 0;
	unsigned int error_counter = NO_ERROR;

	void *space_arr[VSA_INSTANCES];
	vsa_ty *vsa_arr[VSA_INSTANCES];

	size_t initial_size[] = {100, 50, 64, 80};
	size_t predicted_biggest_space_debug[] = {68, 18, 32, 48};
	size_t largest_block_availiable = 0;

	printf("This is %s test\n", test_name);

	AllocateSpace(space_arr, VSA_INSTANCES, initial_size);

	for (i = 0; i < VSA_INSTANCES; i++)
	{
		vsa_arr[i] = VSAInit(space_arr[i], initial_size[i]);

		largest_block_availiable = VSALargestFreeBlock(vsa_arr[i]);
		/*printf("largest- %ld\n", largest_block_availiable);*/
		CmpResults(largest_block_availiable, predicted_biggest_space_debug[i], &error_counter, i, "size_debug");
	}

	SuccessCheck(error_counter, test_name);
	ClearSpace(space_arr, VSA_INSTANCES);
}
/************************TestVSAAlloc******************************************/
void TestVSAAlloc(char *test_name)
{
	size_t i = 0;
	unsigned int error_counter = NO_ERROR;

	void *user_pool = NULL;
	vsa_ty *user_vsa = NULL;
	void *user_allocs[NUM_OF_ALLOCS];

	size_t initial_vsa_size = 300;
	size_t allocations_size[] = {5, 20, 30, 15, 7};
	size_t predicted_biggest_space_debug[] = {268, 244, 204, 156, 124, 100};
	size_t largest_block_availiable = 0;

	printf("This is %s test\n", test_name);

	user_pool = (void *)malloc(initial_vsa_size);

	if (NULL == user_pool)
	{
		puts("system failure");
		return;
	}

	user_vsa = VSAInit(user_pool, initial_vsa_size);

	for (i = 0; i < ARR_SIZE(allocations_size); i++)
	{
		largest_block_availiable = VSALargestFreeBlock(user_vsa);
		CmpResults(predicted_biggest_space_debug[i], largest_block_availiable, &error_counter, i, "size");
		
		user_allocs[i] = VSAAlloc(user_vsa, allocations_size[i]);
	}

	largest_block_availiable = VSALargestFreeBlock(user_vsa);
	CmpResults(predicted_biggest_space_debug[i], largest_block_availiable, &error_counter, i, "size");

	SuccessCheck(error_counter, test_name);
	FREE(user_vsa)
}
/************************TestVSAFree*******************************************/
void TestVSAFree(char *test_name)
{
	size_t i = 0;
	unsigned int error_counter = NO_ERROR;

	void *user_pool = NULL;
	vsa_ty *user_vsa = NULL;
	void *user_allocs[NUM_OF_ALLOCS];


	size_t initial_vsa_size = 300;
	size_t allocations_size[] = {5, 20, 30, 15, 7};
	size_t predicted_biggest_space_debug[] = {268, 244, 204, 156, 124, 100};
	size_t biggest_space_after_free[] = {100, 100, 100, 128, 152, 268};
	size_t largest_block_availiable = 0;

	printf("This is %s test\n", test_name);

	user_pool = (void *)malloc(initial_vsa_size);

	if (NULL == user_pool)
	{
		puts("system failure");
		return;
	}

	user_vsa = VSAInit(user_pool, initial_vsa_size);
	for (i = 0; i < ARR_SIZE(allocations_size); i++)
	{
		largest_block_availiable = VSALargestFreeBlock(user_vsa);
		CmpResults(predicted_biggest_space_debug[i], largest_block_availiable, &error_counter, i, "free size");
		
		user_allocs[i] = VSAAlloc(user_vsa, allocations_size[i]);
	}

	for (i = 0; i < NUM_OF_ALLOCS; i++)
	{
		VSAFree(user_allocs[i]);
		largest_block_availiable = VSALargestFreeBlock(user_vsa);
		/*printf("largest- %ld\n", largest_block_availiable);*/
		CmpResults(biggest_space_after_free[i], largest_block_availiable, &error_counter, i, "free");

	}

	largest_block_availiable = VSALargestFreeBlock(user_vsa);
	/*printf("largest- %ld\n", largest_block_availiable);*/
	CmpResults(biggest_space_after_free[i], largest_block_availiable, &error_counter, i, "free");

	SuccessCheck(error_counter, test_name);
	FREE(user_vsa)
}
/************************TestVSAFree2*******************************************/
void TestVSAFree2(char *test_name)
{
	int i = 0;
	unsigned int error_counter = NO_ERROR;

	void *user_pool = NULL;
	vsa_ty *user_vsa = NULL;

	void *user_allocs[NUM_OF_ALLOCS];

	void *not_user_alloc = &error_counter;

	size_t initial_vsa_size = 300;
	size_t allocations_size[] = {5, 20, 30, 15, 7};
	size_t predicted_biggest_space_debug[] = {268, 244, 204, 156, 124, 100};
	size_t biggest_space_after_free[] = {268, 244, 204, 156, 124};
	size_t largest_block_availiable = 0;

	printf("This is %s test\n", test_name);

	user_pool = (void *)malloc(initial_vsa_size);

	if (NULL == user_pool)
	{
		puts("system failure");
		return;
	}

	user_vsa = VSAInit(user_pool, initial_vsa_size);
	for (i = 0; i < (int)(ARR_SIZE(allocations_size)); i++)
	{
		largest_block_availiable = VSALargestFreeBlock(user_vsa);
		CmpResults(predicted_biggest_space_debug[i], largest_block_availiable, &error_counter, i, "free size");
		
		user_allocs[i] = VSAAlloc(user_vsa, allocations_size[i]);
	}

	/*test if the debug mode works - uncomment the line below*/
	/*VSAFree(not_user_alloc);*/ /*should fail*/

	for (i = NUM_OF_ALLOCS -1; i >= 0; i--)
	{
		VSAFree(user_allocs[i]);
		
		largest_block_availiable = VSALargestFreeBlock(user_vsa);
		/*printf("largest- %ld\n", largest_block_availiable);*/
		CmpResults(biggest_space_after_free[i], largest_block_availiable, &error_counter, i, "free");

	}

	SuccessCheck(error_counter, test_name);
	FREE(user_vsa)
}
/******************************************************************************
*				HELPER FUNCTIONS 
*******************************************************************************/
static void AllocateSpace(void **pointer_arr, size_t size_of_arr, size_t *space)
{
	size_t i = 0;

	for (i = 0; i < size_of_arr; i++)
	{
		pointer_arr[i] = (void *)malloc(space[i]);

		if (NULL == pointer_arr[i])
		{
			ClearSpace(pointer_arr, i+1);
		}
	}
}

static void ClearSpace(void **pointer_arr, size_t size_of_arr)
{
	size_t i = 0;

	for (i = 0; i < size_of_arr; i++)
	{
		free(pointer_arr[i]);
	}
}
