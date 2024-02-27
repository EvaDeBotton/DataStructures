#include <stddef.h>
#include <stdio.h>

int IterativeBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find);
int RecursiveBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find);

int main()
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	int index = 0;
	int i = 0;

	for(i = 0; i < 10; i++)
	{
		index = RecursiveBinarySearch(arr, 10, i + 1);
		printf("expected = %d, result = %d\n", i + 1, index + 1);
	}



	return 0;

}

int IterativeBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find)
{
	int arr_size_local = (int)arr_size;

	int low = 0;
	int high = arr_size_local;
	int mid = (low + high) / 2;

	while(num_to_find != sorted_arr[mid])
	{
		if(high == low + 1)
		{
			return -1;
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

int RecursiveBinarySearch(int *sorted_arr, size_t arr_size, int num_to_find)
{

	int arr_size_local = (int)arr_size;

	int low = 0;
	int high = arr_size_local - 1;
	int mid = (low + high) / 2;

	if(num_to_find == sorted_arr[mid])
	{
		return mid;
	}

	if(num_to_find > sorted_arr[mid])
	{
		return RecursiveBinarySearch(sorted_arr + arr_size / 2, arr_size / 2 + arr_size % 2, num_to_find) + arr_size / 2;
	}
	else
	{
		return RecursiveBinarySearch(sorted_arr, arr_size / 2, num_to_find);
	}

}


