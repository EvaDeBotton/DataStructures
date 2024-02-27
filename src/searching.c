/******************************************************************************
* Project: Binary Search                                                      *
*                                                                             *
* Wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by Arial Holin                                                      *
*                                                                             *
******************************************************************************/
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */

#define NOT_FOUND            -1

/*******************************************************************************
***************************** Iterative Binary Search *************************/

int IterativeBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find)
{
	int low = 0;
	int high = (int)arr_size;
	int mid  = (low + high) / 2;

	assert(NULL != sorted_arr);

	while(num_to_find != sorted_arr[mid])
	{
		if(high == low + 1)
		{
			return NOT_FOUND;
		}

		if(num_to_find > sorted_arr[mid])
		{
			low = mid;
		}
		else
		{
			high = mid; 
		}

		mid = (low + high) / 2;
	}

	return mid;
}

/*******************************************************************************
***************************** Recursive Binary Search *************************/

int RecursiveBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find)
{
	int index = 0;

	int middle = ((int)arr_size) / 2;
	int right_size = ((int)arr_size) / 2;
	int left_size  = ((int)arr_size) / 2 + ((int)arr_size) % 2;

	assert(NULL != sorted_arr);

	if(num_to_find == sorted_arr[middle])
	{
		return middle;
	}

	if(1 == arr_size)
	{
		return NOT_FOUND;
	}

	if(num_to_find > sorted_arr[middle])
	{
		sorted_arr += right_size;

		index = RecursiveBinarySearch(sorted_arr, left_size, num_to_find);

		if(index == NOT_FOUND)
		{
			return NOT_FOUND;
		}

		return (index + right_size);
	}
	else
	{
		index = RecursiveBinarySearch(sorted_arr, right_size, num_to_find);

		return index;
	}
}


