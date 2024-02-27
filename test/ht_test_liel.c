/* liel */
/* review :  */
/*****************************************************************************/
#include <stdio.h> /* __FILE__, __LINE__ */
#include <stddef.h> /* size_t */

#include "../include/ht.h" 
#include "../include/utils_test.h"

#define TEST_ARR_SIZE 10
#define CAPACITY 10

typedef struct person
{
	void *key;
	int value;
}person_ty;
/*****************************************************************************/
static void TestHTCreateDestroy(void);
static void TestHTInsertIsEmptySize(void);
static void TestHTforeach(void);
/****************************************************************************/
int DoesMatch(const void *data, void *param);
size_t HFunc(const void* key);
void* GetKey(const void * data);
int operation(void *data, void *param);
/****************************************************************************/


int main()
{
	TestHTCreateDestroy();
	/*TestHTInsertIsEmptySize();*/
	/*TestHTforeach();*/
	
	return 0;
}

/**************************************************************************/
static void TestHTCreateDestroy(void)
{
	const char *TEST_NAME = "Test HT Create Destroy";

	size_t capacity = CAPACITY;
	
	ht_ty *ht = HTCreate( capacity, HFunc,  GetKey,  DoesMatch);
    HTDestroy(ht);
	PrintTestFinished(TEST_NAME);
}
/*

static void TestHTInsertIsEmptySize(void)
{
	const char *TEST_NAME = "Test HT Insert, remove, Size, is empty";
	size_t capacity = CAPACITY;
	void *key_res = 0;
	int key_res_int = 0;
	size_t key_expected = 0;
    person_ty person[capacity];
	size_t arr[capacity];
	person_ty *person_to_find = NULL;
	double load_f = 0;

	unsigned int i = 0;
	size_t size_res = 0;
	size_t size_expected = capacity-1;
	int is_empty = 1;
	int is_empty_expected = 1;
	ht_ty *ht = HTCreate(capacity, HFunc,  GetKey,  DoesMatch);

	is_empty = HTIsEmpty(ht);

	TestResExpected(is_empty_expected, is_empty, TEST_NAME, __FILE__, __LINE__);

	for(i = 1; i < CAPACITY ; i++)
	{
		arr[i] = i;
		person[i].key = &arr[i];
		person[i].value= 10*arr[i];

		HTInsert(ht , &person[i]);
	}

	is_empty_expected = 0;

	is_empty = HTIsEmpty(ht);

	TestResExpected(is_empty_expected, is_empty, TEST_NAME, __FILE__, __LINE__);

	size_res = HTSize(ht);

	TestResExpected(size_expected, size_res, TEST_NAME, __FILE__, __LINE__);

	person_to_find = HTFind(ht,person[4].key);

	key_res = GetKey((void*)person_to_find);

	key_res_int = *(int*)key_res;
	key_expected = 4;

	TestResExpected(key_expected, key_res_int, TEST_NAME, __FILE__, __LINE__);

	load_f = HTGetLoadFactor(ht);

	printf("load_f before remove is %f\n", load_f);

	HTRemove(ht, person[6].key);
	HTRemove(ht, person[4].key);
	HTRemove(ht, person[2].key);

	load_f = HTGetLoadFactor(ht);

	printf("load_f after remove is %f\n", load_f);

	size_res = HTSize(ht);
	size_expected = capacity - 4;

	TestResExpected(size_expected, size_res, TEST_NAME, __FILE__, __LINE__);

	HTDestroy(ht);
	PrintTestFinished(TEST_NAME);
}*/

/**************************************************************************/
/*static void TestHTforeach(void)
{
	const char *TEST_NAME = "Test HT for each";
	size_t capacity = CAPACITY;
    person_ty person[capacity];
	size_t arr[capacity];
	size_t expected[capacity];
	size_t arr_result[capacity];
	int status = ILRD_SUCCESS;
	int expected_status = ILRD_SUCCESS;
	unsigned int i = 0;
	int int_param = 10;
	void *param = &int_param;
	
	ht_ty *ht = HTCreate(capacity, HFunc,  GetKey,  DoesMatch);

	for(i = 1; i < capacity ; i++)
	{
		arr[i] = i;
		person[i].key = &arr[i];
		person[i].value = 10*arr[i];

		HTInsert(ht , &person[i]);
	}

	status = HTForEach(ht, operation, param);
	TestResExpected(expected_status, status, TEST_NAME, __FILE__, __LINE__);

	for(i = 1; i < capacity; i++)
	{
		expected[i] = 10*10*arr[i];
		arr_result[i] = person[i].value;
		TestResExpected(expected[i], arr_result[i], TEST_NAME, __FILE__, __LINE__);
	}

	HTDestroy(ht);
	PrintTestFinished(TEST_NAME);
}
*/
/*****************************************************************************/
int DoesMatch(const void *data, void *param)
{
	person_ty* data_s = (person_ty*)data;
	person_ty* param_s = (person_ty*)param;

	return(data_s->key == param_s->key);
}

size_t HFunc(const void* key)
{	
	size_t key_int = *(size_t*)key;

	return (key_int);
}

void* GetKey(const void * data)
{	
	person_ty* data_s = (person_ty*)data;

	return ((void*)data_s->key);
}

int operation(void *data, void *param)
{
	person_ty* data_s = (person_ty*)data;
	int param_s = *(int*)param;

	data_s->value *= param_s;
	return(ILRD_SUCCESS);
}

